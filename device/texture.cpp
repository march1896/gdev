#include <algorithm>
#include <cassert>

#include "vmath.h"
#include "texture.h"
#include "bitmap_image.h"

namespace Device {
namespace Texture {
    class TexelType
    {
    public:
        enum class ChannelType: U8
        {
            Red = 0,
            Green = 1,
            Blue = 2,
            Alpha = 3,
            Depth = 4,
            Stencil = 5,
            Count = 6,
        };

        enum class ChannelFormat: U8
        {
            NONE,
            F32,
            U32,
            U8,
            Count
        };

    protected:
        char const* m_name;
        ChannelFormat m_channelFormats[(U8)ChannelType::Count];
        U32 m_numChannels;
        U8 m_channelOffsets[(U8)ChannelType::Count];
        U8 m_size;

    public:
        TexelType(const char* name,
            ChannelFormat rf,
            ChannelFormat gf,
            ChannelFormat bf,
            ChannelFormat af,
            ChannelFormat df,
            ChannelFormat sf)
            : m_name(name)
        {
            // properties
            m_channelFormats[(U8)ChannelType::Red] = rf;
            m_channelFormats[(U8)ChannelType::Green] = gf;
            m_channelFormats[(U8)ChannelType::Blue] = bf;
            m_channelFormats[(U8)ChannelType::Alpha] = af;
            m_channelFormats[(U8)ChannelType::Depth] = df;
            m_channelFormats[(U8)ChannelType::Stencil] = sf;

            // derived properties
            m_numChannels = 0;
            m_size = 0;
            for (U8 channel = 0; channel < (U8)ChannelType::Count; ++channel)
            {
                if (m_channelFormats[channel] != ChannelFormat::NONE)
                {
                    m_numChannels ++;
                }

                m_channelOffsets[channel] = m_size;
                m_size += getChannelWidth((ChannelType)channel);
            }
        }

        inline char const* getName() const
        {
            return m_name;
        }

        inline bool hasChannel(ChannelType const& channel) const
        {
            return m_channelFormats[(U8)channel] != ChannelFormat::NONE;
        }

        inline U32 getNumChannels() const
        {
            return m_numChannels;
        }

        inline ChannelFormat getChannelFormat(ChannelType const& channel) const
        {
            return m_channelFormats[(U8)channel];
        }

        inline U32 getChannelWidth(ChannelType const& channel) const
        {
            switch (getChannelFormat(channel))
            {
                case ChannelFormat::NONE:
                    return 0;
                case ChannelFormat::F32:
                    return 4;
                case ChannelFormat::U32:
                    return 4;
                case ChannelFormat::U8:
                    return 1;

                default:
                    assert(0);
                    return 0;
            }
        }

        inline U32 getSize() const
        {
            return m_size;
        }

        // Returns a [0.0, 1.0] float channel value.
        inline float getChannelFloat(U8* texelAddr, ChannelType channel)
        {
            if (!hasChannel(channel))
            {
                return 0.0f;
            }

            switch (m_channelFormats[(U8)channel])
            {
                case ChannelFormat::NONE:
                    return 0.0f;
                case ChannelFormat::F32:
                    return *reinterpret_cast<float*>(texelAddr + m_channelOffsets[(U8)channel]);
                case ChannelFormat::U32:
                    return *reinterpret_cast<U32*>(texelAddr + m_channelOffsets[(U8)channel]);
                case ChannelFormat::U8:
                    return *reinterpret_cast<U8*>(texelAddr + m_channelOffsets[(U8)channel]);
                default:
                    assert(0);
                    return 0.0f;
            }
        }
    };

    static constexpr TexelType::ChannelFormat TNIL = TexelType::ChannelFormat::NONE;
    static constexpr TexelType::ChannelFormat TF32 = TexelType::ChannelFormat::F32;
    static constexpr TexelType::ChannelFormat TU32 = TexelType::ChannelFormat::U32;
    static constexpr TexelType::ChannelFormat TU8  = TexelType::ChannelFormat::U8;

    static TexelType const s_texelTypes[] =
    {
        TexelType{ "UNKNOWN"            , TNIL ,TNIL ,TNIL ,TNIL ,TNIL ,TNIL }, // UNKNOWN
        TexelType{ "R32G32B32A32_FLOAT" , TF32 ,TF32 ,TF32 ,TF32 ,TNIL ,TNIL }, // R32G32B32A32_FLOAT
        TexelType{ "R32G32B32A32_UINT"  , TU32 ,TU32 ,TU32 ,TU32 ,TNIL ,TNIL }, // R32G32B32A32_UINT
        TexelType{ "R32G32B32_FLOAT"    , TF32 ,TF32 ,TF32 ,TNIL ,TNIL ,TNIL }, // R32G32B32_FLOAT
        TexelType{ "R32G32B32_UINT"     , TU32 ,TU32 ,TU32 ,TNIL ,TNIL ,TNIL }, // R32G32B32_UINT
        TexelType{ "R8G8B8A8_UINT"      , TU8  ,TU8  ,TU8  ,TU8  ,TNIL ,TNIL }, // R8G8B8A8_UINT
        TexelType{ "R8G8B8_UINT"        , TU8  ,TU8  ,TU8  ,TNIL ,TNIL ,TNIL }, // R8G8B8_UINT
        TexelType{ "B8G8R8_UINT"        , TU8  ,TU8  ,TU8  ,TNIL ,TNIL ,TNIL }, // B8G8R8_UINT
        TexelType{ "D32_FLOAT"          , TNIL ,TNIL ,TNIL ,TNIL ,TF32 ,TNIL }, // D32_FLOAT
    };

    void Texture2D::writeTexel(U32 x, U32 y, U8* pNewTexel)
    {
        TexelType const& texelType = s_texelTypes[(U32)m_format];
        U32 texelSize = texelType.getSize();
        U32 offset = (x + m_width * y) * texelSize;
        U8* texelAddr = m_texData + offset;

        std::memcpy(texelAddr, pNewTexel, texelSize);
    }

    U8* Texture2D::readTexel(U32 x, U32 y) const
    {
        TexelType const& texelType = s_texelTypes[(U32)m_format];
        U32 texelSize = texelType.getSize();
        U32 offset = (x + m_width * y) * texelSize;
        return m_texData + offset;
    }

    Vec4f Texture2D::getTexelAsVec4f(U32 x, U32 y) const
    {
        U8* pTexel = readTexel(x, y);

        switch (m_format)
        {
            case TexelFormat::R32G32B32A32_FLOAT:
            {
                Vec4f data = *reinterpret_cast<Vec4f*>(pTexel);
                return data;
            }
            case TexelFormat::R32G32B32A32_UINT:
            {
                Vec4<U32> data = *reinterpret_cast<Vec4<U32>*>(pTexel);
                return Vec4f{(float)data.x/255, (float)data.y/255, (float)data.z/255, (float)data.w/255};
            }
            case TexelFormat::R32G32B32_FLOAT:
            {
                Vec4f data = *reinterpret_cast<Vec4f*>(pTexel);
                return Vec4f{data.x, data.y, data.z, 1.0f};
            }
            case TexelFormat::R32G32B32_UINT:
            {
                Vec4<U32> data = *reinterpret_cast<Vec4<U32>*>(pTexel);
                return Vec4f{(float)data.x/255, (float)data.y/255, (float)data.z/255, 1.0f};
            }
            case TexelFormat::R8G8B8A8_UINT:
            {
                Vec4<U8> data = *reinterpret_cast<Vec4<U8>*>(pTexel);
                return Vec4f{(float)data.x/255, (float)data.y/255, (float)data.z/255, (float)data.w/255};
            }
            case TexelFormat::R8G8B8_UINT:
            {
                Vec4<U8> data = *reinterpret_cast<Vec4<U8>*>(pTexel);
                return Vec4f{(float)data.x/255, (float)data.y/255, (float)data.z/255, 1.0f};
            }
            case TexelFormat::B8G8R8_UINT:
            {
                Vec4<U8> data = *reinterpret_cast<Vec4<U8>*>(pTexel);
                return Vec4f{(float)data.z/255, (float)data.y/255, (float)data.x/255, 1.0f};
            }
            case TexelFormat::D32_FLOAT:
            {
                float data = *reinterpret_cast<float*>(pTexel);
                return Vec4f{data, data, data, data};
            }
            default:
                assert(0);
        }

    }

    void Texture2D::generateMipmap()
    {
        // TODO: to support mipmap, the rasterizer must pass adjacent pixel information to here.
    }

    // [ref](https://en.wikipedia.org/wiki/Bilinear_filtering)
    static Vec4f SampleBilinear(Texture2D const& tex, float u, float v)
    {
        u = u * tex.getWidth() - 0.5f;
        v = v * tex.getHeight() - 0.5f;

        int x = std::floor(u);
        int y = std::floor(v);
        x = clamp(x, 0, (int)tex.getWidth());
        y = clamp(y, 0, (int)tex.getHeight());
        float u_ratio = u - x;
        float v_ratio = v - y;
        float u_opposite = 1 - u_ratio;
        float v_opposite = 1 - v_ratio;

        Vec4f lb = tex.getTexelAsVec4f(x, y);
        Vec4f rb = tex.getTexelAsVec4f(x+1, y);
        Vec4f lt = tex.getTexelAsVec4f(x, y+1);
        Vec4f rt = tex.getTexelAsVec4f(x+1, y+1);

        Vec4f result = interpolate(interpolate(lb, u_opposite, rb, u_ratio), v_opposite,
                                   interpolate(lt, u_opposite, rt, u_ratio), v_ratio);
        return result;
    }

    static Vec4f SampleNearest(Texture2D const& tex, float u, float v)
    {
        u = u * tex.getWidth();
        v = v * tex.getHeight();

        float x = std::nearbyint(u);
        float y = std::nearbyint(v);

        Vec4f result = tex.getTexelAsVec4f(x, y);
        return result;
    }

    Vec4f Sample(Texture2D const& tex, Sampler2D const& samp, Vec2f const& uv)
    {
        if (tex.getStorage() == nullptr)
        {
            return Vec4f{};
        }

        switch (samp.filter)
        {
            case FilterMode::NEAREST:
                return SampleNearest(tex, uv.x, uv.y);
            case FilterMode::LINEAR:
                return SampleBilinear(tex, uv.x, uv.y);
            default:
                return Vec4f{};
        }
    }

    void saveAsBmp(std::string const& filename, Texture2D const& texture)
    {
        U32 width = texture.getWidth();
        U32 height = texture.getHeight();

        bitmap_image image(width, height);
        for (U32 x = 0; x < width; x ++)
        {
            for (U32 y = 0; y < height; y ++)
            {
                U32 const screen_x = x;
                U32 const screen_y = height - y - 1;

                switch (texture.getFormat())
                {
                    case TexelFormat::R32G32B32_FLOAT:
                    {
                        Vec3f const& color = texture.getTexel<Vec3f const>(x, y);
                        image.set_pixel(screen_x, screen_y,
                            clamp(U32(color.x * 255), 0u, 255u),
                            clamp(U32(color.y * 255), 0u, 255u),
                            clamp(U32(color.z * 255), 0u, 255u));
                        break;
                    }
                    case TexelFormat::D32_FLOAT:
                    {
                        float depth = texture.getTexel<float>(x, y);
                        U8 gray = depth * 256;
                        image.set_pixel(screen_x, screen_y, gray, gray, gray);
                        break;
                    }
                    default:
                        assert(0);
                }
            }
        }

        image.save_image(filename.c_str());
    }

    std::ostream &operator<<(std::ostream &stream, Sampler2D const& samp)
    {
        (void)samp;
        stream << "Sampler2D";
        return stream;
    }

    std::ostream &operator<<(std::ostream &stream, Texture2D const& tex)
    {
        (void)tex;
        stream << "Texture2D";
        return stream;
    }

} // namespace Texture
} // namespace Device
