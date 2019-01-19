#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <vector>

#include "vmath.h"

namespace Device {
    // TODO: this may not be handled well in data path
    //struct RGB
    //{
    //    float r, g, b;
    //};

    //RGB operator* (RGB const& a, float m)
    //{
    //    return RGB{a.r * m, a.g * m, a.b * m};
    //}

    //RGB operator* (float m, RGB const& a)
    //{
    //    return RGB{a.r * m, a.g * m, a.b * m};
    //}

    //inline RGB interpolate(RGB const& a, float a_coff, RGB const& b, float b_coff)
    //{
    //    return RGB{
    //        a.r * a_coff + b.r * b_coff,
    //        a.g * a_coff + b.g * b_coff,
    //        a.b * a_coff + b.b * b_coff
    //    };
    //}

    //inline RGB interpolate(RGB const& a, float a_coff, RGB const& b, float b_coff, RGB const& c, float c_coff)
    //{
    //    return RGB{
    //        a.r * a_coff + b.r * b_coff + c.r * c_coff,
    //        a.g * a_coff + b.g * b_coff + c.g * c_coff,
    //        a.b * a_coff + b.b * b_coff + c.b * c_coff
    //    };
    //}

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
#endif // _TEXTURE_H_
