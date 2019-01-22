#include "primitive_assembler.h"

namespace Device {
    PrimitiveAssembler::PrimitiveAssembler()
    {
        addIOPort(Input, std::string("index"), BuiltinType::UINT, Semantic::SV_VertexIndex);
        m_inIndex = getValuePtr(Input, "index");

        addIOPort(Output, std::string("index"), BuiltinType::UINT, Semantic::SV_VertexIndex);
        m_outIndex = getValuePtr(Output, "index");
    }

    bool PrimitiveAssembler::isOneInOneOut() const
    {
        return true;
    }

    void PrimitiveAssembler::runOne()
    {
        // TODO: isOneInOneOut is not true for non-list primitives
        // NOTE: assume triangle list for now.
        m_outIndex->write(m_inIndex->read());

        // std::cout << *m_inIndex << " " << *m_outIndex << std::endl;
    }

    void PrimitiveAssembler::comsumeOneInput()
    {
        // never be here
        assert(0);
    }

    bool PrimitiveAssembler::hasPendingOutput() const
    {
        // never be here
        return false;
    }

    void PrimitiveAssembler::produceOneOutput()
    {
        // never be here
        assert(0);
    }
} // namespace Device
