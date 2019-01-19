
#include "semantic.h"

namespace Device {

    Semantic const Semantic::Position0 = Semantic{ Semantic::Position, 0};
    Semantic const Semantic::Position1 = Semantic{ Semantic::Position, 1};
    Semantic const Semantic::Position2 = Semantic{ Semantic::Position, 2};
    Semantic const Semantic::Position3 = Semantic{ Semantic::Position, 3};

    Semantic const Semantic::Color0 = Semantic{ Semantic::Color, 0};
    Semantic const Semantic::Color1 = Semantic{ Semantic::Color, 1};
    Semantic const Semantic::Color2 = Semantic{ Semantic::Color, 2};
    Semantic const Semantic::Color3 = Semantic{ Semantic::Color, 3};

    Semantic const Semantic::Normal0 = Semantic{ Semantic::Normal, 0};
    Semantic const Semantic::Normal1 = Semantic{ Semantic::Normal, 1};
    Semantic const Semantic::Normal2 = Semantic{ Semantic::Normal, 2};
    Semantic const Semantic::Normal3 = Semantic{ Semantic::Normal, 3};

    Semantic const Semantic::Texcoord0 = Semantic{ Semantic::Texcoord, 0};
    Semantic const Semantic::Texcoord1 = Semantic{ Semantic::Texcoord, 1};
    Semantic const Semantic::Texcoord2 = Semantic{ Semantic::Texcoord, 2};
    Semantic const Semantic::Texcoord3 = Semantic{ Semantic::Texcoord, 3};

    Semantic const Semantic::SV_Position = Semantic{ Semantic::SYSTEM_VALUE, 1};
    Semantic const Semantic::SV_Depth = Semantic{ Semantic::SYSTEM_VALUE, 2};
    Semantic const Semantic::SV_Target = Semantic{ Semantic::SYSTEM_VALUE, 3};
    Semantic const Semantic::SV_VertexIndex = Semantic{ Semantic::SYSTEM_VALUE, 4};

} // namespace Device
