#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <vector>
#include <iostream>

#include "vmath.h"

namespace Device { namespace Texture {
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

    std::ostream &operator<<(std::ostream &stream, Sampler2D const& samp);

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
        R8G8B8_UINT,
        B8G8R8_UINT,
        D32_FLOAT,
    };

    class Texture2D
    {
    protected:
        TexelFormat m_format;
        U32 m_width;
        U32 m_height;
        U8* m_texData;
        U8* m_mipmapData;

    public:
        Texture2D()
            : m_format{TexelFormat::UNKNOWN}
            , m_width{0}
            , m_height{0}
            , m_texData{nullptr}
            , m_mipmapData{nullptr}
        {
        }

        Texture2D(TexelFormat format, U32 width, U32 height, U8* storage)
            : m_format{format}
            , m_width{width}
            , m_height{height}
            , m_texData{storage}
            , m_mipmapData{nullptr}
        {
        }

        ~Texture2D()
        {
            if (m_mipmapData != nullptr)
            {
                free(m_mipmapData);
                m_mipmapData = nullptr;
            }
        }

        inline void setFormat(TexelFormat format) { m_format = format; }

        inline TexelFormat getFormat() const { return m_format; }

        inline void setWidth(U32 width) { m_width = width; }

        inline U32 getWidth() const { return m_width; }

        inline void setHeigth(U32 height) { m_height = height; }

        inline U32 getHeight() const { return m_height; }

        inline void setSize(U32 width, U32 height) { m_width = width; m_height = height; }

        inline void setStorage(U8* storage) { m_texData = storage; }

        inline U8* getStorage() const { return m_texData; }

        template <typename T>
        void setTexel(U32 x, U32 y, T const& newTexel)
        {
            writeTexel(x, y, (U8*)&newTexel);
        }

        template <typename T>
        T getTexel(U32 x, U32 y) const
        {
            T result = *reinterpret_cast<T*>(readTexel(x, y));
            return result;
        }

        Vec4f getTexelAsVec4f(U32 x, U32 y) const;

        void writeTexel(U32 x, U32 y, U8* pNewTexel);

        U8* readTexel(U32 x, U32 y) const;

        void generateMipmap();
    };

    std::ostream &operator<<(std::ostream &stream, Texture2D const& tex);

    // [Ref](http://www.shaderific.com/glsl-functions/)
    // From OpenGL texture() spec, it returns a vec4
    Vec4f Sample(Texture2D const& tex, Sampler2D const& samp, Vec2f const& uv);

    void saveAsBmp(std::string const& filename, Texture2D const& colorTarget);

} // namespace Texture
} // namespace Device
#endif // _TEXTURE_H_
