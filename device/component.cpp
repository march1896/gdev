#include <algorithm>

#include "component.h"

namespace Device {
    U32 SizeOf(Type const& type)
    {
        switch (type)
        {
            case Type::FLOAT:
                return TypeTrait<Type::FLOAT>::Width;
            case Type::FLOAT2:
                return TypeTrait<Type::FLOAT2>::Width;
            case Type::FLOAT3:
                return TypeTrait<Type::FLOAT3>::Width;
            case Type::FLOAT4:
                return TypeTrait<Type::FLOAT4>::Width;
            case Type::FLOAT4X4:
                return TypeTrait<Type::FLOAT4X4>::Width;
            case Type::HALF:
                return TypeTrait<Type::HALF>::Width;
            case Type::DOUBLE:
                return TypeTrait<Type::DOUBLE>::Width;
            case Type::INT:
                return TypeTrait<Type::INT>::Width;
            case Type::UINT:
                return TypeTrait<Type::UINT>::Width;
            default:
                assert(0);
        }

        return 0u;
    }

    template <typename T>
    static void interpolateAs(Value* out, Value const* a, float u, Value const* b, float v)
    {
        T _a = a->readAs<T>();
        T _b = b->readAs<T>();
        T _o = _a * u + _b * v;
        out->write((U8*)&_o);
    }

    template <typename T>
    static void interpolateAs(Value* out, Value const* a, float u, Value const* b, float v, Value const* c, float w)
    {
        T _a = a->readAs<T>();
        T _b = b->readAs<T>();
        T _c = c->readAs<T>();
        T _o = _a * u + _b * v + _c * w;
        out->write((U8*)&_o);
    }

    void Value::interpolate(Value const* a, float u, Value const* b, float v)
    {
        switch (m_type)
        {
            case Type::FLOAT:
            {
                interpolateAs<float>(this, a, u, b, v);
                break;
            }
            case Type::FLOAT2:
            {
                interpolateAs<Vec2f>(this, a, u, b, v);
                break;
            }
            case Type::FLOAT3:
            {
                interpolateAs<Vec3f>(this, a, u, b, v);
                break;
            }
            case Type::FLOAT4:
            {
                interpolateAs<Vec4f>(this, a, u, b, v);
                break;
            }
            case Type::HALF:
            {
                // TODO:
                assert(0);
                break;
            }
            case Type::DOUBLE:
            {
                interpolateAs<double>(this, a, u, b, v);
                break;
            }
            case Type::INT:
            {
                interpolateAs<int>(this, a, u, b, v);
                break;
            }
            case Type::UINT:
            {
                interpolateAs<U32>(this, a, u, b, v);
                break;
            }
            default:
            {
                assert(0);
            }
        }
    }

    void Value::interpolate(Value const* a, float u, Value const* b, float v, Value const* c, float w)
    {
        switch (m_type)
        {
            case Type::FLOAT:
            {
                interpolateAs<float>(this, a, u, b, v, c, w);
                break;
            }
            case Type::FLOAT2:
            {
                interpolateAs<Vec2f>(this, a, u, b, v, c, w);
                break;
            }
            case Type::FLOAT3:
            {
                interpolateAs<Vec3f>(this, a, u, b, v, c, w);
                break;
            }
            case Type::FLOAT4:
            {
                interpolateAs<Vec4f>(this, a, u, b, v, c, w);
                break;
            }
            case Type::HALF:
            {
                // TODO:
                assert(0);
                break;
            }
            case Type::DOUBLE:
            {
                interpolateAs<double>(this, a, u, b, v, c, w);
                break;
            }
            case Type::INT:
            {
                interpolateAs<int>(this, a, u, b, v, c, w);
                break;
            }
            case Type::UINT:
            {
                interpolateAs<U32>(this, a, u, b, v, c, w);
                break;
            }
            default:
            {
                assert(0);
            }
        }
    }

    std::ostream &operator<<(std::ostream &stream, Value const& value)
    {
        switch (value.type())
        {
            case Type::FLOAT:
            {
                float cppValue = value.readAs<float>();
                stream << cppValue;
                break;
            }
            case Type::FLOAT2:
            {
                Vec2f cppValue = value.readAs<Vec2f>();
                stream << cppValue;
                break;
            }
            case Type::FLOAT3:
            {
                Vec3f cppValue = value.readAs<Vec3f>();
                stream << cppValue;
                break;
            }
            case Type::FLOAT4:
            {
                Vec4f cppValue = value.readAs<Vec4f>();
                stream << cppValue;
                break;
            }
            case Type::FLOAT4X4:
            {
                Mat44f cppValue = value.readAs<Mat44f>();
                stream << cppValue;
                break;
            }
            case Type::HALF:
            {
                assert(0);
                break;
            }
            case Type::DOUBLE:
            {
                double cppValue = value.readAs<double>();
                stream << cppValue;
                break;
            }
            case Type::INT:
            {
                int cppValue = value.readAs<int>();
                stream << cppValue;
                break;
            }
            case Type::UINT:
            {
                uint cppValue = value.readAs<uint>();
                stream << cppValue;
                break;
            }
            default:
            {
                assert(0);
            }
        }

        return stream;
    }

    Comp::Comp()
        : m_types{}
        , m_values{}
        , m_symbols{}
        , m_semantics{}
        , ctr_totalConsumed{0}
        , ctr_totalProduced{0}
    {
    }

    U32 Comp::addIOPort(IOType io, std::string const& name, Type const& type, Semantic const& semantic)
    {
        Types& types = m_types[io];
        Values& values = m_values[io];
        Symbols& symbols = m_symbols[io];
        Semantics& semantics = m_semantics[io];

        symbols.push_back(name);
        types.push_back(type);
        semantics.push_back(semantic);
        values.push_back(Value{type});

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
            Type const& type = getType(io, portIndex);
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

    Type Comp::getType(Comp::IOType io, U32 location) const
    {
        return m_types[io][location];
    }

    Type Comp::getType(Comp::IOType io, Semantic const& semantic) const
    {
        U32 location = getLocation(io, semantic);
        return getType(io, location);
    }

    Type Comp::getType(Comp::IOType io, std::string const& name) const
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

    Value* Comp::getValuePtr(Comp::IOType io, U32 location)
    {
        return &m_values[io][location];
    }

    Value* Comp::getValuePtr(Comp::IOType io, Semantic const& semantic)
    {
        U32 location = getLocation(io, semantic);
        return getValuePtr(io, location);
    }

    Value* Comp::getValuePtr(Comp::IOType io, std::string const& name)
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
