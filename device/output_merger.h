#ifndef _OUTPUT_MERGER_H_
#define _OUTPUT_MERGER_H_

#include "vmath.h"
#include "texture.h"
#include "component.h"

namespace Device {
    class OutputMerger: public Comp
    {
    protected:
        U32 m_width;
        U32 m_height;

        std::vector<Vec3f> m_colorStorage;
        std::vector<float> m_depthStorage;

        // TODO: handle alpha?
        Texture::Texture2D m_colorTarget;
        Texture::Texture2D m_depthTarget;
        // TODO: stencil target?

        Value* m_inPosition;
        Value* m_inColor;

    public:
        OutputMerger();

        void resize(U32 width, U32 height);

        void setWidth(U32 width);

        void setHeight(U32 height);

        U32 getWidth() const;

        U32 getHeight() const;

        // Component interface begin
        bool isOneInOneOut() const;

        void runOne();

        void comsumeOneInput();

        bool hasPendingOutput() const;

        void produceOneOutput();
        // Component interface end
        
        // output
        void presentToBmp() const;
    };
} // namespace Device
#endif // _OUTPUT_MERGER_H_
