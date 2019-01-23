#ifndef _PRIMITIVE_ASSEMBLER_H_
#define _PRIMITIVE_ASSEMBLER_H_
#include "prelude.h"
#include "component.h"

namespace Device {
    class PrimitiveAssembler: public Comp
    {
    protected:
        BuiltinValueRef* m_inIndex;

        BuiltinValueRef* m_outIndex;

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
