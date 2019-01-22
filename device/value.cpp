#include <algorithm>

#include "value.h"

namespace Device {
    U32 SizeOf(BuiltinType const& type)
    {
        switch (type)
        {
            case BuiltinType::FLOAT:
                return sizeof(float);
            case BuiltinType::FLOAT2:
                return sizeof(Vec2f);
            case BuiltinType::FLOAT3:
                return sizeof(Vec3f);
            case BuiltinType::FLOAT4:
                return sizeof(Vec4f);
            case BuiltinType::FLOAT4X4:
                return sizeof(Mat44f);
            case BuiltinType::HALF:
                assert(0);
                return 0;
            case BuiltinType::DOUBLE:
                return sizeof(double);
            case BuiltinType::INT:
                return sizeof(int);
            case BuiltinType::UINT:
                return sizeof(U32);
            case BuiltinType::SAMPLER2D:
                return sizeof(Texture::Sampler2D);
            case BuiltinType::TEXTURE2D:
                return sizeof(Texture::Texture2D);
        }
    }

    void interpolate_inplace(BuiltinValueRef& out, BuiltinValueRef const& a, float u, BuiltinValueRef const& b, float v)
    {
        assert(out.type() == a.type());
        assert(out.type() == b.type());
        switch (out.type())
        {
            case BuiltinType::FLOAT:
            {
                ::interpolate_inplace(out.readAs<float>(), a.readAs<float>(), u, b.readAs<float>(), v);
                break;
            }
            case BuiltinType::FLOAT2:
            {
                ::interpolate_inplace(out.readAs<Vec2f>(), a.readAs<Vec2f>(), u, b.readAs<Vec2f>(), v);
                break;
            }
            case BuiltinType::FLOAT3:
            {
                ::interpolate_inplace(out.readAs<Vec3f>(), a.readAs<Vec3f>(), u, b.readAs<Vec3f>(), v);
                break;
            }
            case BuiltinType::FLOAT4:
            {
                ::interpolate_inplace(out.readAs<Vec4f>(), a.readAs<Vec4f>(), u, b.readAs<Vec4f>(), v);
                break;
            }
            case BuiltinType::HALF:
            {
                // TODO:
                assert(0);
                break;
            }
            case BuiltinType::DOUBLE:
            {
                ::interpolate_inplace(out.readAs<double>(), a.readAs<double>(), u, b.readAs<double>(), v);
                break;
            }
            case BuiltinType::INT:
            {
                ::interpolate_inplace(out.readAs<int>(), a.readAs<int>(), u, b.readAs<int>(), v);
                break;
            }
            case BuiltinType::UINT:
            {
                ::interpolate_inplace(out.readAs<U32>(), a.readAs<U32>(), u, b.readAs<U32>(), v);
                break;
            }
            default:
            {
                assert(0);
            }
        }
    }

    void interpolate_inplace(BuiltinValueRef& out, BuiltinValueRef const& a, float u, BuiltinValueRef const& b, float v, BuiltinValueRef const& c, float w)
    {
        assert(out.type() == a.type());
        assert(out.type() == b.type());
        assert(out.type() == c.type());

        switch (out.type())
        {
            case BuiltinType::FLOAT:
            {
                ::interpolate_inplace(out.readAs<float>(), a.readAs<float>(), u, b.readAs<float>(), v, c.readAs<float>(), w);
                break;
            }
            case BuiltinType::FLOAT2:
            {
                ::interpolate_inplace(out.readAs<Vec2f>(), a.readAs<Vec2f>(), u, b.readAs<Vec2f>(), v, c.readAs<Vec2f>(), w);
                break;
            }
            case BuiltinType::FLOAT3:
            {
                ::interpolate_inplace(out.readAs<Vec3f>(), a.readAs<Vec3f>(), u, b.readAs<Vec3f>(), v, c.readAs<Vec3f>(), w);
                break;
            }
            case BuiltinType::FLOAT4:
            {
                ::interpolate_inplace(out.readAs<Vec4f>(), a.readAs<Vec4f>(), u, b.readAs<Vec4f>(), v, c.readAs<Vec4f>(), w);
                break;
            }
            case BuiltinType::HALF:
            {
                // TODO:
                assert(0);
                break;
            }
            case BuiltinType::DOUBLE:
            {
                ::interpolate_inplace(out.readAs<double>(), a.readAs<double>(), u, b.readAs<double>(), v, c.readAs<double>(), w);
                break;
            }
            case BuiltinType::INT:
            {
                ::interpolate_inplace(out.readAs<int>(), a.readAs<int>(), u, b.readAs<int>(), v, c.readAs<int>(), w);
                break;
            }
            case BuiltinType::UINT:
            {
                ::interpolate_inplace(out.readAs<U32>(), a.readAs<U32>(), u, b.readAs<U32>(), v, c.readAs<U32>(), w);
                break;
            }
            default:
            {
                assert(0);
            }
        }
    }

    U32 SizeOf(StructType const& type)
    {
        return type.getSize();
    }

    std::ostream &operator<<(std::ostream &stream, BuiltinValueRef const& value)
    {
        switch (value.type())
        {
            case BuiltinType::FLOAT:
            {
                float cppValue = value.readAs<float>();
                stream << cppValue;
                break;
            }
            case BuiltinType::FLOAT2:
            {
                Vec2f cppValue = value.readAs<Vec2f>();
                stream << cppValue;
                break;
            }
            case BuiltinType::FLOAT3:
            {
                Vec3f cppValue = value.readAs<Vec3f>();
                stream << cppValue;
                break;
            }
            case BuiltinType::FLOAT4:
            {
                Vec4f cppValue = value.readAs<Vec4f>();
                stream << cppValue;
                break;
            }
            case BuiltinType::FLOAT4X4:
            {
                Mat44f cppValue = value.readAs<Mat44f>();
                stream << cppValue;
                break;
            }
            case BuiltinType::HALF:
            {
                assert(0);
                break;
            }
            case BuiltinType::DOUBLE:
            {
                double cppValue = value.readAs<double>();
                stream << cppValue;
                break;
            }
            case BuiltinType::INT:
            {
                int cppValue = value.readAs<int>();
                stream << cppValue;
                break;
            }
            case BuiltinType::UINT:
            {
                U32 cppValue = value.readAs<U32>();
                stream << cppValue;
                break;
            }
            case BuiltinType::SAMPLER2D:
            {
                Texture::Sampler2D sampler = value.readAs<Texture::Sampler2D>();
                stream << sampler;
                break;
            }
            case BuiltinType::TEXTURE2D:
            {
                Texture::Texture2D tex = value.readAs<Texture::Texture2D>();
                stream << tex;
                break;
            }
            default:
            {
                assert(0);
            }
        }

        return stream;
    }

    StructType::StructType()
        : m_fieldOffsets{}
        , m_fieldSemantics{}
        , m_fieldTypes{}
        , m_size{0}
    {
    }

    void StructType::updateOffsets()
    {
        m_size = 0;

        m_fieldOffsets.clear();
        for (U32 fieldIndex = 0; fieldIndex < m_fieldSemantics.size(); ++fieldIndex)
        {
            m_fieldOffsets.push_back(m_size);

            m_size += SizeOf(m_fieldTypes[fieldIndex]);
        }
    }

    // Returns the field index.
    U32 StructType::addField(Semantic const& semantic, BuiltinType const& type)
    {
        for (U32 fieldIndex = 0; fieldIndex < m_fieldSemantics.size(); ++fieldIndex)
        {
            Semantic const& fieldSemantic = m_fieldSemantics[fieldIndex];

            if (semantic == fieldSemantic)
            {
                // use semantic as binding idertifier, do not permit override.
                assert(0);
            }
        }

        m_fieldSemantics.push_back(semantic);
        m_fieldTypes.push_back(type);

        // TODO: optimize, we only need to update the last one in this condition.
        updateOffsets();

        return m_fieldTypes.size() - 1;
    }

    U32 StructType::getFieldOffset(U32 fieldIndex) const
    {
        assert(fieldIndex < m_fieldOffsets.size());
        return m_fieldOffsets[fieldIndex];
    }

    U32 StructType::getFieldOffset(Semantic const& semantic) const
    {
        U32 fieldIndex = getFieldIndex(semantic);
        return getFieldOffset(fieldIndex);
    }

    BuiltinType StructType::getFieldType(U32 fieldIndex) const
    {
        return m_fieldTypes[fieldIndex];
    }

    BuiltinType StructType::getFieldType(Semantic const& semantic) const
    {
        U32 fieldIndex = getFieldIndex(semantic);
        return getFieldType(fieldIndex);
    }


    U32 StructType::numFields() const
    {
        return m_fieldSemantics.size();
    }

    U32 StructType::getSize() const
    {
        return m_size;
    }

    void StructType::reset()
    {
        m_fieldOffsets.clear();
        m_fieldSemantics.clear();
        m_fieldTypes.clear();
        m_size = 0;
    }

    U32 StructType::getFieldIndex(Semantic const& semantic) const
    {
        auto const& itr = std::find(m_fieldSemantics.begin(), m_fieldSemantics.end(), semantic);
        return itr - m_fieldSemantics.begin();
    }

    Semantic StructType::getFieldSemantic(U32 fieldIndex) const
    {
        return m_fieldSemantics[fieldIndex];
    }


} // namespace Device

