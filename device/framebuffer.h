#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#include <vector>

#include "vmath.h"
#include "texture.h"

namespace Device {

    // TODO: remove this file.
    class FrameBuffer
    {
    protected:
        U32 m_width;
        U32 m_height;

        Surface<Vec3f> m_color;
        Surface<float> m_depth;

    public:
        FrameBuffer();

        FrameBuffer(U32 width, U32 height);

        void resize(U32 width, U32 height);

        inline U32 getWidth() const
        {
            return m_width;
        }

        inline U32 getHeight() const
        {
            return m_height;
        }

        inline void setColor(U32 x, U32 y, Vec3f const& color)
        {
            m_color.setPixel(x, y, color);
        }

        inline Vec3f getColor(U32 x, U32 y) const
        {
            return m_color.getPixel(x, y);
        }

        inline void setDepth(U32 x, U32 y, float depth)
        {
            m_depth.setPixel(x, y, depth);
        }

        inline float getDepth(U32 x, U32 y) const
        {
            return m_depth.getPixel(x, y);
        }

        void clear();

        void present() const;
    };
}
#endif // _FRAMEBUFFER_H_
