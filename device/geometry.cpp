#include "geometry.h"

namespace Device {
    Line2D setupLine(Vec2f const& va, Vec2f const& vb)
    {
        // calculation the line equation, counter-clock-wise
        // point (x, y) in the left side of line(va, vb)
        // cross_product (x-va.x, y-va.y) (vb.x-va.x, vb.y-va.y) < 0
        // (x-va.x)*(vb.y-va.y) - (y-va.y)*(vb.x-va.x) < 0
        // (vb.y-va.y)*x + (va.x-vb.x)*y + va.x*(va.y-vb.y) - va.y*(va.x-vb.x) < 0
        // (vb.y-va.y)*x + (va.x-vb.x)*y + a.y*b.x - a.x*b.y < 0

        Line2D res;
        res.a = vb.y-va.y;
        res.b = va.x-vb.x;
        res.c = va.y*vb.x - va.x*vb.y;
        return res;
    }

    float evalLine(Line2D const& line, Vec2f const& p)
    {
        return line.a * p.x + line.b * p.y + line.c;
    }

    Triangle2D setupTriangle(Vec2f const& va, Vec2f const& vb, Vec2f const& vc)
    {
        Triangle2D triangle;

        // copy vertex
        triangle.va = va;
        triangle.vb = vb;
        triangle.vc = vc;

        // setup edge
        triangle.ab = setupLine(va, vb);
        triangle.bc = setupLine(vb, vc);
        triangle.ca = setupLine(vc, va);

        return triangle;
    }

    bool insideTriangle(Triangle2D const& triangle, Vec2f const& vp)
    {
        bool const leftToAB = evalLine(triangle.ab, vp) < 0.0f;
        bool const leftToBC = evalLine(triangle.bc, vp) < 0.0f;
        bool const leftToCA = evalLine(triangle.ca, vp) < 0.0f;

        bool inside = leftToAB && leftToBC && leftToCA;
        return inside;
    }

    BaryCentricCoff calcBaryCentricCoordinates(Triangle2D const& triangle, Vec2f const& vp)
    {
        // https://www.rose-hulman.edu/class/cs/csse351/m10/triangle_fill.pdf
        // BaryCentric(u, v, w) = u * A + v * B + w * C
        //
        //   xp = u * xa + v * xb + w * xc
        //   yp = u * ya + v * yb + w * yc
        //   u + v + w = 1
        BaryCentricCoff coff;
        coff.u = evalLine(triangle.bc, vp) / evalLine(triangle.bc, triangle.va);
        coff.v = evalLine(triangle.ca, vp) / evalLine(triangle.ca, triangle.vb);
        coff.w = evalLine(triangle.ab, vp) / evalLine(triangle.ab, triangle.vc);

        return coff;
    }
} // namespace Device
