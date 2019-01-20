#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <vector>

#include "vmath.h"

namespace Device {

    enum class FilterMode
    {
        NEAREST,
        LINEAR,
        NEAREST_MIPMAP_NEAREST,
        LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_LINEAR,
    };

    enum class AddressMode
    {
        WRAP,
        MIRROR,
        CLAMP,
        BORDER,
    };

    struct Sampler2D
    {
        FilterMode filter;
        AddressMode addressU;
        AddressMode addressV;
    };

    // [ref](https://docs.microsoft.com/en-us/windows/desktop/api/dxgiformat/ne-dxgiformat-dxgi_format)
    // Note: a very sub-set of types is supported.
    enum class TexelFormat
    {
        UNKNOWN,
        R32G32B32A32_FLOAT,
        R32G32B32A32_UINT,
        R32G32B32_FLOAT,
        R32G32B32_UINT,
        R8G8B8A8_UINT,
        D32_FLOAT,
    };

    template <TexelFormat tf>
    struct TexelTrait
    {
        using CppType = void*;
        static constexpr U32 Size = sizeof(void*);
        static constexpr U32 NumChannels = 0;
    };

#define def_texel_trait(texelFormat, cppType, numChannel) \
    template <>\
    struct TexelTrait<texelFormat> {\
        using CppType = cppType;\
        static constexpr U32 Width = sizeof(cppType);\
        static constexpr U32 NumChannels = numChannel;\
    };

    def_texel_trait(TexelFormat::R32G32B32A32_FLOAT, Vec4<float>, 4)
    def_texel_trait(TexelFormat::R32G32B32A32_UINT, Vec4<U32>, 4)
    def_texel_trait(TexelFormat::R32G32B32_FLOAT, Vec3<float>, 3)
    def_texel_trait(TexelFormat::R32G32B32_UINT, Vec4<U32>, 3)
    def_texel_trait(TexelFormat::R8G8B8A8_UINT, Vec4<U8>, 4)
    def_texel_trait(TexelFormat::D32_FLOAT, float, 1)
#undef def_texel_trait

    U32 TexelSize(TexelFormat texelFormat);

    U32 TexelNumChannels(TexelFormat texelFormat);

    class Texture2D
    {
    protected:
        TexelFormat m_format;
        U32 m_width;
        U32 m_height;

    public:
        Texture2D()
            : m_format{TexelFormat::UNKNOWN}
            , m_width{0}
            , m_height{0}
        {
        }

        inline void setFormat(TexelFormat format) { m_format = format; }

        inline TexelFormat getFormat() const { return m_format; }

        inline void setWidth(U32 width) { m_width = width; }

        inline U32 getWidth() const { return m_width; }

        inline void setHeigth(U32 height) { m_height = height; }

        inline U32 getHeight() const { return m_height; }

        void setTexel(U8* pStorage, U32 x, U32 y, U8* pNewTexel)
        {
            U32 texelSize = TexelSize(m_format);
            U32 offset = (x + m_width * y) * texelSize;
            U8* texelAddr = pStorage + offset;

            std::memcpy(texelAddr, pNewTexel, texelSize);
        }

        U8* getTexel(U8* pStorage, U32 x, U32 y) const
        {
            U32 texelSize = TexelSize(m_format);
            U32 offset = (x + m_width * y) * texelSize;
            return pStorage + offset;
        }
    };

    template <typename T>
    class Surface
    {
    protected:
        TexelFormat m_format;
        U32 m_width;
        U32 m_height;
        std::vector<U8> m_buffer;

    public:
        Surface()
            : m_format{TexelFormat::UNKNOWN}
            , m_width{0}
            , m_height{0}
        {
            U32 const tSize = TexelSize(m_format);
            m_buffer.resize(m_width * m_height * tSize);
        }

        Surface(TexelFormat format, U32 width, U32 height)
            : m_format{format}
            , m_width(width)
            , m_height(height)
        {
            U32 const tSize = TexelSize(m_format);
            m_buffer.resize(m_width * m_height * tSize);
        }

        inline void resize(U32 width, U32 height)
        {
            if (m_width == width && m_height == height)
            {
                return;
            }
            m_width = width;
            m_height = height;
            U32 const tSize = TexelSize(m_format);
            m_buffer.resize(m_width * m_height * tSize);
        }

        inline U32 getWidth() const
        {
            return m_width;
        }

        inline U32 getHeight() const
        {
            return m_height;
        }

        inline void write(U32 x, U32 y, U8* pNewTexel)
        {
            U8* texelAddr = read(x, y);
            std::memcpy(texelAddr, pNewTexel, TexelSize(m_format));
        }

        inline U8* read(U32 x, U32 y)
        {
            return m_buffer.data() + (x + m_width * y) * TexelSize(m_format);
        }
    };

    //template <typename T>
    //using Texture2D = Surface<T>;

    // [ref](https://en.wikipedia.org/wiki/Bilinear_filtering)
    template <typename T>
    float SampleBilinear(U8* pTex, Texture2D const& tex, float u, float v)
    {
        u = u * tex.getWidth() - 0.5f;
        v = v * tex.getHeight() - 0.5f;
        int x = std::floor(u);
        int y = std::floor(v);
        float u_ratio = u - x;
        float v_ratio = v - y;
        float u_opposite = 1 - u_ratio;
        float v_opposite = 1 - v_ratio;

        T lb = *reinterpret_cast<T*>(tex.getTexel(x, y ));

        float result = (tex.getTexel(x, y) * u_opposite + tex.getTexel(x+1, y) * u_ratio) * v_opposite + (tex.getTexel(x, y+1) * u_opposite + tex.getTexel(x+1, y+1) * u_ratio) * v_ratio;
        return result;
    }

    U8* Sample(Texture2D const& tex, Sampler2D const& samp, Vec2f uv)
    {
        switch (samp.filter)
        {
            case FilterMode::NEAREST:
                return nullptr;
            case FilterMode::LINEAR:
                return nullptr;
            default:
                return nullptr;
        }
    }

    void saveAsBmp(std::string const& filename, Surface const& colorTarget);

    void saveAsBmp(std::string const& filename, Surface const& depthTarget);

}
#endif // _TEXTURE_H_
