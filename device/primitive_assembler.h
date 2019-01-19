#ifndef _PRIMITIVE_ASSEMBLER_H_
#define _PRIMITIVE_ASSEMBLER_H_
#include "vmath.h"
#include "component.h"

namespace Device {
    class PrimitiveAssembler: public Comp
    {
    protected:
        Value* m_inIndex;

        Value* m_outIndex;

    public:
        PrimitiveAssembler();

        bool isOneInOneOut() const;

        void runOne();

        void comsumeOneInput();

        bool hasPendingOutput() const;

        void produceOneOutput();
    };

} // namespace Device
#endif // _PRIMITIVE_ASSEMBLER_H_
