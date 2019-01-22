#include <algorithm>

#include "component.h"

namespace Device {
    Comp::Comp()
        : m_types{}
        , m_values{}
        , m_symbols{}
        , m_semantics{}
        , ctr_totalConsumed{0}
        , ctr_totalProduced{0}
    {
    }

    U32 Comp::addIOPort(IOType io, std::string const& name, BuiltinType const& type, Semantic const& semantic)
    {
        Types& types = m_types[io];
        Values& values = m_values[io];
        Symbols& symbols = m_symbols[io];
        Semantics& semantics = m_semantics[io];

        symbols.push_back(name);
        types.push_back(type);
        semantics.push_back(semantic);
        values.push_back(BuiltinType{type});

        return symbols.size() - 1;
    }

    U32 Comp::getNumPorts(IOType io) const
    {
        return m_types[io].size();
    }

    U32 Comp::getLinearMemLayout(IOType io, std::vector<U32>& portOffsets) const
    {
        U32 portOffset = 0;
        U32 numPorts = getNumPorts(io);
        for (U32 portIndex = 0; portIndex < numPorts; portIndex ++)
        {
            BuiltinType const& type = getType(io, portIndex);
            portOffsets.push_back(portOffset);
            portOffset += SizeOf(type);
        }
        return portOffset;
    }

    bool Comp::isRequired(IOType io, U32 location) const
    {
        Semantic const& semantic = getSemantic(io, location);
        return isRequired(io, semantic);
    }

    bool Comp::isRequired(IOType io, Semantic const& semantic) const
    {
        (void)io;
        return semantic.name == Semantic::SYSTEM_VALUE;
    }

    bool Comp::isRequired(IOType io, std::string const& name) const
    {
        Semantic const& semantic = getSemantic(io, name);
        return isRequired(io, semantic);
    }

    template <typename Container>
    static U32 findLocation(Container const& container, typename Container::value_type const& value)
    {
        typename Container::const_iterator itr = std::find(container.begin(), container.end(), value);

        if (itr == container.end())
        {
            // TODO: handle this
            assert(0);
        }

        return std::distance(container.begin(), itr);
    }

    U32 Comp::getLocation(Comp::IOType io, std::string const& name) const
    {
        return findLocation(m_symbols[io], name);
    }

    U32 Comp::getLocation(Comp::IOType io, Semantic const& semantic) const
    {
        return findLocation(m_semantics[io], semantic);
    }

    BuiltinType Comp::getType(Comp::IOType io, U32 location) const
    {
        return m_types[io][location];
    }

    BuiltinType Comp::getType(Comp::IOType io, Semantic const& semantic) const
    {
        U32 location = getLocation(io, semantic);
        return getType(io, location);
    }

    BuiltinType Comp::getType(Comp::IOType io, std::string const& name) const
    {
        U32 location = getLocation(io, name);
        return getType(io, location);
    }

    std::string Comp::getName(Comp::IOType io, U32 location) const
    {
        return m_symbols[io][location];
    }

    std::string Comp::getName(Comp::IOType io, Semantic const& semantic) const
    {
        U32 location = getLocation(io, semantic);
        return getName(io, location);
    }

    Semantic Comp::getSemantic(Comp::IOType io, U32 location) const
    {
        return m_semantics[io][location];
    }

    Semantic Comp::getSemantic(Comp::IOType io, std::string const& name) const
    {
        U32 location = getLocation(io, name);
        return getSemantic(io, location);
    }

    BuiltinValueRef* Comp::getValuePtr(Comp::IOType io, U32 location)
    {
        return &m_values[io][location];
    }

    BuiltinValueRef* Comp::getValuePtr(Comp::IOType io, Semantic const& semantic)
    {
        U32 location = getLocation(io, semantic);
        return getValuePtr(io, location);
    }

    BuiltinValueRef* Comp::getValuePtr(Comp::IOType io, std::string const& name)
    {
        U32 location = getLocation(io, name);
        return getValuePtr(io, location);
    }

    //template <typename Component, typename InStream, typename OutStream>
    //void runComp(Component& comp, InStream& inStream, OutStream& outStream)
    //{
    //    outStream.clearData();

    //    U32 curInNumPorts = comp.getNumPorts(Comp::Input);
    //    U32 curOutNumPorts = comp.getNumPorts(Comp::Output);

    //    for (U32 inBufIndex = 0; inBufIndex < inStream.getLength(); ++inBufIndex)
    //    {
    //        for (U32 inLocation = 0; inLocation < curInNumPorts; ++inLocation)
    //        {
    //            Semantic const& semantic = comp.getSemantic(Comp::Input, inLocation);
    //            U8* addr = inStream.getAddr(semantic, inBufIndex);

    //            if (addr == nullptr)
    //            {
    //                if (comp.isRequired(Comp::Input, inLocation))
    //                {
    //                    // component is required but not represent in previous component.
    //                    assert(0);
    //                }
    //                else
    //                {
    //                    // TODO: make default value then bind
    //                }
    //            }
    //            else
    //            {
    //                Value* value = comp.getValuePtr(Comp::Input, inLocation);
    //                value->bind(addr);
    //            }
    //        }

    //        U32 outBufIndex = outStream.pushData();
    //        for (U32 outLocation = 0; outLocation < curOutNumPorts; ++outLocation)
    //        {
    //            Semantic const& semantic = comp.getSemantic(Comp::Output, outLocation);
    //            U8* addr = outStream.getAddr(semantic, outBufIndex);

    //            Value* value = comp.getValuePtr(Comp::Output, outLocation);
    //            value->bind(addr);
    //        }

    //        // All input and output are bound, run the component.
    //        comp.run();
    //    }
    //}

} // namespace Device
