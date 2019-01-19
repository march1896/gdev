#include "framebuffer.h"
#include "bitmap_image.h"

namespace Device {
    FrameBuffer::FrameBuffer()
    {
    }

    FrameBuffer::FrameBuffer(U32 width, U32 height)
        : m_width(width)
        , m_height(height)
        , m_color(width, height)
        , m_depth(width, height)
    {
    }

    void FrameBuffer::resize(U32 width, U32 height)
    {
        m_width = width;
        m_height = height;
        m_color.resize(width, height);
        m_depth.resize(width, height);
    }

    void FrameBuffer::clear()
    {
        // TODO:
    }

    void FrameBuffer::present() const
    {
        saveAsBmp("fb_color.bmp", m_color);
        saveAsBmp("fb_depth.bmp", m_depth);
    }
}
