#include <algorithm>

#include "vmath.h"
#include "rasterizer.h"

namespace {
    using namespace Device;

    // Plane equation.
    // P(x, y, z) = A*x + B*y + C*z + D;
    struct TrianglePlaneMeta
    {
        StructType structure;
        StructValueRef A;
        StructValueRef B;
        StructValueRef C;
        StructValueRef D;
        U8* m_storage;

        TrianglePlaneMeta(StructType const& t, StructValueRef const& a, StructValueRef const& b, StructValueRef const& c)
            : structure(t)
            , A(structure, nullptr)
            , B(structure, nullptr)
            , C(structure, nullptr)
            , D(structure, nullptr)
        {
            m_storage = (U8*)malloc(structure.getSize() * 4);

            A.bind(m_storage + structure.getSize() * 0);
            B.bind(m_storage + structure.getSize() * 1);
            C.bind(m_storage + structure.getSize() * 2);
            D.bind(m_storage + structure.getSize() * 3);

            // TODO: continue
            for (U32 fieldIdx = 0; fieldIdx < structure.numFields(); ++fieldIdx)
            {

            }
        }

        ~TrianglePlaneMeta()
        {
            assert(m_storage);
            free(m_storage);
        }
    };
}

namespace Device {

    Rasterizer::Rasterizer()
        : m_width(1)
        , m_height(1)
    {
        // raster input is connected to primitive assember output.
        addIOPort(Input, std::string("vtx_index"), BuiltinType::UINT, Semantic::SV_VertexIndex);
        m_inVtxIdx = getValuePtr(Input, "vtx_index");

        m_triIndex = 0;
    }

    void Rasterizer::resize(U32 width, U32 height)
    {
        m_width = width;
        m_height = height;
    }

    void Rasterizer::setWidth(U32 width)
    {
        m_width = width;
    }

    void Rasterizer::setHeight(U32 height)
    {
        m_height = height;
    }

    U32 Rasterizer::getWidth() const
    {
        return m_width;
    }

    U32 Rasterizer::getHeight() const
    {
        return m_height;
    }

    // TODO: implement ddx/ddy
    std::vector<Vec2f> Rasterizer::rasterizeTriangle(Vec4f const& va, Vec4f const& vb, Vec4f const& vc)
    {
        std::vector<Vec2f> pixels;

        // setup triangle
        m_curTriangle = setupTriangle(*(Vec2f*)&va, *(Vec2f*)&vb, *(Vec2f*)&vc);

        // setup NDC(normalized device coordinates) bounding box.
        AABB<float> ndcBox;
        ndcBox.xmin = clamp(std::min({va.x, vb.x, vc.x}), -1.0f, 1.0f);
        ndcBox.xmax = clamp(std::max({va.x, vb.x, vc.x}), -1.0f, 1.0f);
        ndcBox.ymin = clamp(std::min({va.y, vb.y, vc.y}), -1.0f, 1.0f);
        ndcBox.ymax = clamp(std::max({va.y, vb.y, vc.y}), -1.0f, 1.0f);

        U32 const width = m_width;
        U32 const height = m_height;

        // setup screen space bonding box.
        int xmin = std::floor(ndcBox.xmin * width); // left
        int xmax = std::ceil(ndcBox.xmax * width); // right
        int ymin = std::floor(ndcBox.ymin * height); // bottom
        int ymax = std::ceil(ndcBox.ymax * height); // top

        // Note: the nearest odd number
        for (int x = xmin / 2 * 2 + 1; x < xmax; x += 2)
        {
            for (int y = ymin / 2 * 2 + 1; y < ymax; y += 2)
            {
                Vec2f pixel{float(x)/width, float(y)/height};

                // TODO: near/far clipping? this can only be done after interpolation.
                if (insideTriangle(m_curTriangle, pixel))
                {
                    pixels.push_back(pixel);
                }
            }
        }

        return pixels;
    }

    void Rasterizer::rasterizeLine(Vec2f const& va, Vec2f const& b)
    {
        // TODO:
        (void)va;
        (void)b;
    }

    void Rasterizer::bindVSOutput(FifoStream& fifoStream)
    {
        m_vsOutBuffer = StreamBuffer{fifoStream};

        StructType const& structure = m_vsOutBuffer.getElementStruct();
        m_vsOutPositionChannel = structure.getFieldIndex(Semantic::SV_Position);

        if (m_vsOutPositionChannel == structure.numFields())
        {
            // Vertex shader does not provide SV_Position as output.
            assert(0);
        }

        // do perspective divide.
        for (U32 bufIdx = 0; bufIdx < m_vsOutBuffer.getLength(); ++ bufIdx)
        {
            Vec4f& vPos = *(Vec4f*)m_vsOutBuffer.getElement(bufIdx).getFieldAddr(m_vsOutPositionChannel);

            vPos.x /= vPos.w;
            vPos.y /= vPos.w;
            vPos.z /= vPos.w;
        }
    }

    void Rasterizer::adjustOutputPorts(Comp const& nextComp)
    {
        // clean up old state
        m_types[Comp::Output].clear();
        m_symbols[Comp::Output].clear();
        m_values[Comp::Output].clear();
        m_semantics[Comp::Output].clear();

        // setup new port stats.
        U32 numPorts = nextComp.getNumPorts(Comp::Input);
        for (U32 portIdx = 0; portIdx < numPorts; ++portIdx)
        {
            Semantic const& semantic = nextComp.getSemantic(Comp::Input, portIdx);
            BuiltinType const& type = nextComp.getType(Comp::Input, portIdx);
            std::string const& name = nextComp.getName(Comp::Input, portIdx);

            addIOPort(Comp::Output, name, type, semantic);
        }
    }

    bool Rasterizer::isOneInOneOut() const
    {
        return false;
    }

    void Rasterizer::runOne()
    {
        assert(0);
    }

    void Rasterizer::comsumeOneInput()
    {
        m_triVtxIndices[m_triIndex++] = m_inVtxIdx->readAs<U32>();

        if (m_triIndex == 3)
        {
            assert(!hasPendingOutput());

            Vec4f* va = (Vec4f*)m_vsOutBuffer.getElement(m_triVtxIndices[0]).getFieldAddr(m_vsOutPositionChannel);
            Vec4f* vb = (Vec4f*)m_vsOutBuffer.getElement(m_triVtxIndices[1]).getFieldAddr(m_vsOutPositionChannel);
            Vec4f* vc = (Vec4f*)m_vsOutBuffer.getElement(m_triVtxIndices[2]).getFieldAddr(m_vsOutPositionChannel);

            m_triPending = rasterizeTriangle(*va, *vb, *vc);
            m_triProcessed = 0;

            m_triIndex = 0;
        }
    }

    bool Rasterizer::hasPendingOutput() const
    {
        return m_triProcessed < m_triPending.size();
    }

    void Rasterizer::produceOneOutput()
    {
        assert(m_triProcessed < m_triPending.size());

        Vec2f pixel = m_triPending[m_triProcessed++];
        BaryCentricCoff const& coff = calcBaryCentricCoordinates(m_curTriangle, pixel);

        // TODO: move this after ps and vsout binding.
        StructType const& structure = m_vsOutBuffer.getElementStruct();

        // Note: assume rasterizer out ports are bound.
        // Note: rasterizer out ports are dynamically constructed, which matchs psIn
        U32 numOutPorts = getNumPorts(Output);
        StreamBuffer::Element element_a = m_vsOutBuffer.getElement(m_triVtxIndices[0]);
        StreamBuffer::Element element_b = m_vsOutBuffer.getElement(m_triVtxIndices[1]);
        StreamBuffer::Element element_c = m_vsOutBuffer.getElement(m_triVtxIndices[2]);

        for (U32 outPortIdx = 0; outPortIdx < numOutPorts; ++outPortIdx)
        {
            BuiltinType const& type = getType(Comp::Output, outPortIdx);

            // TODO: cache below values
            BuiltinValueRef va{type}, vb{type}, vc{type};
            U32 channel = structure.getFieldIndex(getSemantic(Comp::Output, outPortIdx));

            va.bind(element_a.getFieldAddr(channel));
            vb.bind(element_b.getFieldAddr(channel));
            vc.bind(element_c.getFieldAddr(channel));

            BuiltinValueRef* out = getValuePtr(Comp::Output, outPortIdx);
            interpolate_inplace(*out, va, coff.u, vb, coff.v, vc, coff.w);

            // debug
            //{
            //    if (outPortIdx == 0)
            //    {
            //        // output position
            //        // std::cout << *out << std::endl; // " " <<  va << " " << vb << " " << vc << std::endl;
            //        Vec3f pos = out->readAs<Vec3f>();
            //        std::cout << U32(pos.x * width) << " " << U32(pos.y * height) << std::endl;
            //    }
            //}
        }
    }
}
