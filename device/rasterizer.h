#ifndef _RASTERIZER_H_
#define _RASTERIZER_H_

#include "vmath.h"
#include "buffer.h"
#include "geometry.h"
#include "component.h"

namespace Device {

    class Rasterizer: public Comp
    {
    protected:
        U32 m_width;
        U32 m_height;

        // rasterizer internal state
        StreamBuffer m_vsOutBuffer;
        U32 m_vsOutPositionChannel;

        Value* m_inVtxIdx;

        U32 m_triVtxIndices[3];
        U32 m_triIndex;
        std::vector<BaryCentricCoff> m_triPending;
        U32 m_triProcessed;

    public:
        Rasterizer();

        void resize(U32 width, U32 height);

        void setWidth(U32 width);

        void setHeight(U32 height);

        U32 getWidth() const;

        U32 getHeight() const;

        std::vector<BaryCentricCoff> rasterizeTriangle(Vec4f const& va, Vec4f const& vb, Vec4f const& vc);

        void rasterizeLine(Vec2f const& va, Vec2f const& b);

    public:
        ////////////////////////////////////////////////////
        // component interface
        void bindVSOutput(FifoStream& fifoStream);

        // Adjust this component's output ports to next components input port.
        void adjustOutputPorts(Comp const& nextComp);

        bool isOneInOneOut() const;

        void runOne();

        void comsumeOneInput();

        bool hasPendingOutput() const;

        void produceOneOutput();
    };

}
#endif // _RASTERIZER_H_
