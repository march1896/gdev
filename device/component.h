#ifndef _COMPONENT_H_
#define _COMPONENT_H_
#include <string>
#include <vector>
#include <cstring>
#include <cassert>
#include <climits>
#include <iostream>

#include "vmath.h"
#include "semantic.h"

namespace Device {

    // TODO: move this to standalong file?
    enum class Type
    {
        UNKNOWN,

        FLOAT,
        FLOAT2,
        FLOAT3,
        FLOAT4,

        FLOAT4X4,

        HALF,

        DOUBLE,

        INT,

        UINT,
    };

    U32 SizeOf(Type const& type);

    template <Type type>
    struct TypeTrait {
        // TODO: make default unknown
        using CppType = void*;
        static constexpr U32 Width = sizeof(void*);
    };

#define def_type_trait(elementType, cppType) \
    template <> \
    struct TypeTrait<elementType> { \
        using CppType = cppType; \
        static constexpr U32 Width = sizeof(cppType); \
    };

    def_type_trait(Type::FLOAT,  float)
    def_type_trait(Type::FLOAT2, Vec2f)
    def_type_trait(Type::FLOAT3, Vec3f)
    def_type_trait(Type::FLOAT4, Vec4f)
    def_type_trait(Type::FLOAT4X4, Mat44f)
    def_type_trait(Type::UINT,  unsigned int)
    def_type_trait(Type::INT, int)
    def_type_trait(Type::HALF, short)

#undef def_type_trait

    class Value
    {
    protected:
        Type const m_type;
        U8* m_addr;

    public:
        Value(Type const& type)
            : m_type(type)
            , m_addr(nullptr)
        {}

        inline Type type() const { return m_type; }

        inline U8* read() const { return m_addr; }

        template <typename T>
        inline T readAs() const { return *reinterpret_cast<T*>(m_addr); }

        inline void write(U8* new_val) { std::memcpy(m_addr, new_val, SizeOf(m_type)); }

        inline void bind(U8* addr) { m_addr = addr; }

        void interpolate(Value const* a, float u, Value const* b, float v);

        void interpolate(Value const* a, float u, Value const* b, float v, Value const* c, float w);
    };

    std::ostream &operator<<(std::ostream &stream, Value const& ob);

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
        typedef std::vector<Type> Types;
        typedef std::vector<Value> Values;
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
        U32 addIOPort(IOType io, std::string const& name, Type const& type, Semantic const& semantic);

        U32 getNumPorts(IOType io) const;

        U32 getLinearMemLayout(IOType io, std::vector<U32>& portOffsets) const;

        U32 getLocation(IOType io, std::string const& name) const;

        U32 getLocation(IOType io, Semantic const& semantic) const;

        bool isRequired(IOType io, U32 port) const;

        bool isRequired(IOType io, Semantic const& semantic) const;

        bool isRequired(IOType io, std::string const& name) const;

        Type getType(IOType io, U32 port) const;

        Type getType(IOType io, Semantic const& semantic) const;

        Type getType(IOType io, std::string const& name) const;

        std::string getName(IOType io, U32 port) const;

        std::string getName(IOType io, Semantic const& semantic) const;

        Semantic getSemantic(IOType io, U32 port) const;

        Semantic getSemantic(IOType io, std::string const& name) const;

        Value* getValuePtr(IOType io, U32 port);

        Value* getValuePtr(IOType io, Semantic const& semantic);

        Value* getValuePtr(IOType io, std::string const& name);
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
                    Value* value = comp.getValuePtr(io, port);
                    value->bind(nullptr);
                }
            }
            else
            {
                Value* value = comp.getValuePtr(io, port);
                U8* addr = data.getData(channel);
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
