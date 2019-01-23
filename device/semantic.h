#ifndef _SEMANTIC_H_
#define _SEMANTIC_H_
#include "prelude.h"

namespace Device {

    struct Semantic
    {
        enum EName
        {
            INVALID,
            Position, // 3 dementional position
            Color,
            Normal,
            Texcoord,

            SYSTEM_VALUE,
        };

        EName name;
        U32 index;

        Semantic()
            : name(Semantic::INVALID), index(0)
        {
        }

        Semantic(EName _name, U32 _index)
            : name(_name) , index(_index)
        {
        }

        inline bool operator== (Semantic const& other) const
        {
            return name == other.name && index == other.index;
        }

        inline bool operator!= (Semantic const& other) const
        {
            return !(*this == other);
        }

        static Semantic const Position0;
        static Semantic const Position1;
        static Semantic const Position2;
        static Semantic const Position3;

        static Semantic const Color0;
        static Semantic const Color1;
        static Semantic const Color2;
        static Semantic const Color3;

        static Semantic const Normal0;
        static Semantic const Normal1;
        static Semantic const Normal2;
        static Semantic const Normal3;

        static Semantic const Texcoord0;
        static Semantic const Texcoord1;
        static Semantic const Texcoord2;
        static Semantic const Texcoord3;

        static Semantic const SV_Position;    // SV: output of vertex shader, required by rasterizer
        static Semantic const SV_Depth;       // SV: output of pixel shader, required by depth test
        static Semantic const SV_Target;      // SV: output of pixel shader, pixel color
        static Semantic const SV_VertexIndex; // SV: output of primitive assember, required by rasterizer

        static Semantic const UNKNOWN;
    };

} // namespace Device
#endif // _SEMANTIC_H_
