#include <iostream>
#include "output_merger.h"

namespace Device {
    OutputMerger::OutputMerger()
        : m_width(0)
        , m_height(0)
        , m_colorStorage{}
        , m_depthStorage{}
        , m_colorTarget{Texture::TexelFormat::R32G32B32_FLOAT, m_width, m_height, nullptr}
        , m_depthTarget{Texture::TexelFormat::D32_FLOAT, m_width, m_height, nullptr}
    {
        addIOPort(Input, std::string("position"), Type::FLOAT3, Semantic::SV_Position);
        addIOPort(Input, std::string("color"), Type::FLOAT3, Semantic::SV_Target);
        m_inPosition = getValuePtr(Input, "position");
        m_inColor = getValuePtr(Input, "color");
    }

    void OutputMerger::resize(U32 width, U32 height)
    {
        if (m_width == width && m_height == height)
        {
            return;
        }

        m_width = width;
        m_height = height;

        m_colorStorage.resize(width * height);
        m_depthStorage.resize(width * height);

        m_colorTarget.setSize(width, height);
        m_depthTarget.setSize(width, height);

        m_colorTarget.setStorage((U8*)m_colorStorage.data());
        m_depthTarget.setStorage((U8*)m_depthStorage.data());
    }

    void OutputMerger::setWidth(U32 width)
    {
        resize(width, m_height);
    }

    void OutputMerger::setHeight(U32 height)
    {
        resize(m_width, height);
    }

    U32 OutputMerger::getWidth() const
    {
        return m_width;
    }

    U32 OutputMerger::getHeight() const
    {
        return m_height;
    }

    bool OutputMerger::isOneInOneOut() const
    {
        return false;
    }

    void OutputMerger::runOne()
    {
        // never be here
        assert(0);
    }

    void OutputMerger::comsumeOneInput()
    {
        // TODO: early Z
        Vec4f pos = m_inPosition->readAs<Vec4f>();

        int screen_x = std::nearbyint(pos.x * m_width);
        int screen_y = std::nearbyint(pos.y * m_height);

        // screen_x, screen_y is around a odd number, save to divide by 2.
        screen_x = (screen_x + m_width) / 2;
        screen_y = (screen_y + m_height) / 2;

        // map [-1, 1] to [1, 0]
        float depth = -(pos.z - 1.0f) / 2.0f;
        bool zTestResult = depth > m_depthTarget.getTexel<float>(screen_x, screen_y);
        if (zTestResult)
        {
            m_depthTarget.setTexel(screen_x, screen_y, depth);
        }

        if (zTestResult)
        {
            Vec3f color = m_inColor->readAs<Vec3f>();
            m_colorTarget.setTexel(screen_x, screen_y, color);
        }
    }

    bool OutputMerger::hasPendingOutput() const
    {
        return false;
    }

    void OutputMerger::produceOneOutput()
    {
        assert(0);
    }

    void OutputMerger::presentToBmp() const
    {
        Texture::saveAsBmp("fb_color.bmp", m_colorTarget);
        Texture::saveAsBmp("fb_depth.bmp", m_depthTarget);
    }
} // namespace Device
