#ifndef _COMPONENT_H_
#define _COMPONENT_H_
#include <string>
#include <vector>
#include <cstring>
#include <cassert>
#include <climits>
#include <iostream>

#include "vector.h"
#include "value.h"
#include "semantic.h"
#include "texture.h"

namespace Device {

    class Comp
    {
    public:
        enum IOType
        {
            Input,
            Output,
            IOEnd,
        };

    protected:
        typedef std::vector<BuiltinType> Types;
        typedef std::vector<BuiltinValueRef> Values;
        typedef std::vector<Semantic> Semantics;
        typedef std::vector<std::string> Symbols;

        Types m_types[IOEnd];
        Values m_values[IOEnd];
        Symbols m_symbols[IOEnd];
        Semantics m_semantics[IOEnd];

    public:
        // counters
        U32 ctr_totalConsumed;
        U32 ctr_totalProduced;

    public:
        Comp();

        // Optimize: IOType could be statically bound at most of the time.

        ///////////////////////////////////////////////////////////
        // IO interfaces, start
        //
        // Return a unique location of this io port
        U32 addIOPort(IOType io, std::string const& name, BuiltinType const& type, Semantic const& semantic);

        U32 getNumPorts(IOType io) const;

        U32 getLinearMemLayout(IOType io, std::vector<U32>& portOffsets) const;

        U32 getLocation(IOType io, std::string const& name) const;

        U32 getLocation(IOType io, Semantic const& semantic) const;

        bool isRequired(IOType io, U32 port) const;

        bool isRequired(IOType io, Semantic const& semantic) const;

        bool isRequired(IOType io, std::string const& name) const;

        BuiltinType getType(IOType io, U32 port) const;

        BuiltinType getType(IOType io, Semantic const& semantic) const;

        BuiltinType getType(IOType io, std::string const& name) const;

        std::string getName(IOType io, U32 port) const;

        std::string getName(IOType io, Semantic const& semantic) const;

        Semantic getSemantic(IOType io, U32 port) const;

        Semantic getSemantic(IOType io, std::string const& name) const;

        BuiltinValueRef* getValuePtr(IOType io, U32 port);

        BuiltinValueRef* getValuePtr(IOType io, Semantic const& semantic);

        BuiltinValueRef* getValuePtr(IOType io, std::string const& name);
        // IO interfaces, end
        ///////////////////////////////////////////////////////////

        ///////////////////////////////////////////////////////////
        // Runtime interfaces, start

        // Returns true if the component comsumes one input and then produce one output at each run.
        bool isOneInOneOut() const;

        // Assume the input and output are bound
        // This function only works if isOneInOneOut() is true
        void runOne();

        // Assume all input ports are bound, comsume one element.
        void comsumeOneInput();

        bool hasPendingOutput() const;

        // Assume all output ports are bound, produce one element.
        void produceOneOutput();
        // Runtime interfaces, end
        ///////////////////////////////////////////////////////////
    };

    template <typename Component, typename IOStream>
    std::vector<U32> mapCompPortToStreamChannel(Component const& comp, IOStream const& stream, Comp::IOType io)
    {
        std::vector<U32> portToChannel;

        U32 numPorts = comp.getNumPorts(io);
        portToChannel.resize(numPorts);

        for (U32 port = 0; port < numPorts; ++port)
        {
            Semantic const& semantic = comp.getSemantic(io, port);
            U32 channelIndex = stream.getChannelIndex(semantic);

            if (channelIndex == stream.numChannels())
            {
                channelIndex = UINT_MAX;
            }

            portToChannel[port] = channelIndex;
        }
        return portToChannel;
    }

    template <typename Component, typename ElementData>
    void bindPortsToStreamData(
        Component& comp,
        Comp::IOType io,
        ElementData const& data,
        std::vector<U32> const& portToChannelMapping
        )
    {
        U32 numPorts = comp.getNumPorts(io);

        for (U32 port = 0u; port < numPorts; ++port)
        {
            U32 channel = portToChannelMapping[port];

            if (channel == UINT_MAX)
            {
                if (comp.isRequired(io, port))
                {
                    // component is required but not represent in previous component.
                    assert(0);
                }
                else
                {
                    // TODO: make default value then bind
                    BuiltinValueRef* value = comp.getValuePtr(io, port);
                    value->bind(nullptr);
                }
            }
            else
            {
                BuiltinValueRef* value = comp.getValuePtr(io, port);
                U8* addr = data.getFieldAddr(channel);
                value->bind(addr);
                // std::cout << io << " " << port << " " << channel << " " << (unsigned long)value << " " << value->readAs<Vec3f>() << std::endl;
            }
        }
    }

    template <typename Component, typename InStream, typename OutStream>
    void runComp(Component& comp, InStream& inStream, OutStream& outStream)
    {
        // setup input, bind inStream to inputs
        std::vector<U32> inLocationToStreamChannel = mapCompPortToStreamChannel(comp, inStream, Comp::Input);
        std::vector<U32> outLocationToStreamChannel = mapCompPortToStreamChannel(comp, outStream, Comp::Output);

        if (comp.isOneInOneOut())
        {
            while (!inStream.isEmpty() && !outStream.isFull())
            {
                // if is OneInOneOut, we could bind input and output at the same time,
                // this may avoid component internal state copy/paste.
                typename InStream::Element inData = inStream.front();
                bindPortsToStreamData(comp, Comp::Input, inData, inLocationToStreamChannel);
                typename OutStream::Element outData = outStream.pushData();
                bindPortsToStreamData(comp, Comp::Output, outData, outLocationToStreamChannel);

                comp.runOne();

                inStream.popData();

                comp.ctr_totalConsumed ++;
                comp.ctr_totalProduced ++;
            }
        }
        else
        {
            while (!inStream.isEmpty() && !comp.hasPendingOutput())
            {
                // bind inStream to input locations
                typename InStream::Element inData = inStream.front();
                bindPortsToStreamData(comp, Comp::Input, inData, inLocationToStreamChannel);

                comp.comsumeOneInput();

                inStream.popData();
                comp.ctr_totalConsumed ++;
            }

            while (!outStream.isFull() && comp.hasPendingOutput())
            {
                // bind outStream to out locations
                typename OutStream::Element outData = outStream.pushData();
                bindPortsToStreamData(comp, Comp::Output, outData, outLocationToStreamChannel);

                comp.produceOneOutput();
                comp.ctr_totalProduced ++;
            }
        }
    }
} // namespace Device
#endif // _COMPONENT_H_
