#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <vector>
#include <iostream>

#include "vmath.h"

namespace Device {
    template <typename ColorFormat>
    class Surface
    {
    protected:
        U32 m_width;
        U32 m_height;
        std::vector<ColorFormat> m_buffer;

    public:
        Surface()
            : m_width(0)
            , m_height(0)
        {
            m_buffer.resize(m_width * m_height);
        }

        Surface(U32 width, U32 height)
            : m_width(width)
            , m_height(height)
        {
            m_buffer.resize(m_width * m_height);
        }

        void resize(U32 width, U32 height)
        {
            if (m_width == width && m_height == height)
            {
                return;
            }
            m_width = width;
            m_height = height;
            m_buffer.resize(m_width * m_height);
        }

        U32 getWidth() const
        {
            return m_width;
        }

        U32 getHeight() const
        {
            return m_height;
        }

        void setPixel(U32 x, U32 y, ColorFormat const& color)
        {
            m_buffer[x + m_width * y] = color;
        }

        ColorFormat getPixel(U32 x, U32 y) const
        {
            return m_buffer[x + m_width * y];
        }
    };

    void saveAsBmp(std::string const& filename, Surface<Vec3f> colorTarget);

    void saveAsBmp(std::string const& filename, Surface<float> depthTarget);
}

namespace Device {
namespace Texture {
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
        D32_FLOAT,
    };

    class Texture2D
    {
    protected:
        TexelFormat m_format;
        U32 m_width;
        U32 m_height;
        U8* m_storage;

    public:
        Texture2D()
            : m_format{TexelFormat::UNKNOWN}
            , m_width{0}
            , m_height{0}
            , m_storage{nullptr}
        {
        }

        Texture2D(TexelFormat format, U32 width, U32 height, U8* storage)
            : m_format{format}
            , m_width{width}
            , m_height{height}
            , m_storage{storage}
        {
        }

        inline void setFormat(TexelFormat format) { m_format = format; }

        inline TexelFormat getFormat() const { return m_format; }

        inline void setWidth(U32 width) { m_width = width; }

        inline U32 getWidth() const { return m_width; }

        inline void setHeigth(U32 height) { m_height = height; }

        inline U32 getHeight() const { return m_height; }

        inline void setSize(U32 width, U32 height) { m_width = width; m_height = height; }

        inline void setStorage(U8* storage) { m_storage = storage; }

        inline U8* getStorage() const { return m_storage; }

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

        void writeTexel(U32 x, U32 y, U8* pNewTexel);

        U8* readTexel(U32 x, U32 y) const;
    };

    std::ostream &operator<<(std::ostream &stream, Texture2D const& tex);

    // [ref](https://en.wikipedia.org/wiki/Bilinear_filtering)
    template <typename T>
    T SampleBilinear(Texture2D const& tex, float u, float v)
    {
        u = u * tex.getWidth() - 0.5f;
        v = v * tex.getHeight() - 0.5f;

        int x = std::floor(u);
        int y = std::floor(v);
        float u_ratio = u - x;
        float v_ratio = v - y;
        float u_opposite = 1 - u_ratio;
        float v_opposite = 1 - v_ratio;

        T lb = tex.getTexel<T>(x, y);
        T rb = tex.getTexel<T>(x+1, y);
        T lt = tex.getTexel<T>(x, y+1);
        T rt = tex.getTexel<T>(x+1, y+1);

        T result = interpolate(interpolate(lb, u_opposite, rb, u_ratio), v_opposite,
                               interpolate(lt, u_opposite, rt, u_ratio), v_ratio);
        return result;
    }

    template <typename T>
    T SampleNearest(Texture2D const& tex, float u, float v)
    {
        u = u * tex.getWidth() - 0.5f;
        v = v * tex.getHeight() - 0.5f;

        float x = std::nearbyint(u);
        float y = std::nearbyint(v);

        T result = tex.getTexel<T>(x, y);
        return result;
    }

    template <typename T>
    T Sample(Texture2D const& tex, Sampler2D const& samp, Vec2f const& uv)
    {
        if (tex.getStorage() == nullptr)
        {
            return T{};
        }

        switch (samp.filter)
        {
            case FilterMode::NEAREST:
                return SampleNearest<T>(tex, uv.x, uv.y);
            case FilterMode::LINEAR:
                return SampleBilinear<T>(tex, uv.x, uv.y);
            default:
                return T{};
        }
    }

    void saveAsBmp(std::string const& filename, Texture2D const& colorTarget);
} // namespace Texture
} // namespace Device
#endif // _TEXTURE_H_
