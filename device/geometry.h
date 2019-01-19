#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "vmath.h"

namespace Device {
    struct Line2D
    {
        // E(x, y) = a*x + b*y + c
        float a, b, c;
    };

    struct Triangle2D
    {
        Vec2f va, vb, vc;
        Line2D ab, bc, ca;
    };

    Line2D setupLine(Vec2f const& va, Vec2f const& vb);

    // >0 means left to this line.
    // <0 means right to this line.
    float evalLine(Line2D const& line, Vec2f const& p);

    Triangle2D setupTriangle(Vec2f const& va, Vec2f const& vb, Vec2f const& vc);

    bool insideTriangle(Triangle2D const& triangle, Vec2f const& vp);

    struct BaryCentricCoff
    {
        float u, v, w;
    };

    BaryCentricCoff calcBaryCentricCoordinates(Triangle2D const& tmeta, Vec2f const& vp);

    template <typename T>
    struct AABB
    {
        // axis-aligned bounding box
        T xmin, xmax, ymin, ymax;
    };

} // namespace Device

#endif // _GEOMETRY_H_
