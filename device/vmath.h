#ifndef _VMATH_H_
#define _VMATH_H_

namespace Device {
    // common math operations
    template <typename T>
    T interpolate(T const& a, float a_coff, T const& b, float b_coff)
    {
        return a * a_coff + b * b_coff;
    }

    template <typename T>
    T interpolate(T const& a, float a_coff, T const& b, float b_coff, T const& c, float c_coff)
    {
        return a * a_coff + b * b_coff + c * c_coff;
    }

    template <typename T>
    T clamp(const T& target, T const& lower, T const& upper)
    {
        T result = target;

        if (result > upper)
            result = upper;
        else if (result < lower)
            result = lower;

        return result;
    }

    // inplace interpolate
    template <typename T>
    void interpolate_inplace(T& out, T const& a, float a_coff, T const& b, float b_coff)
    {
        out = a * a_coff + b * b_coff;
    }

    template <typename T>
    void interpolate_inplace(T& out, T const& a, float a_coff, T const& b, float b_coff, T const& c, float c_coff)
    {
        out = a * a_coff + b * b_coff + c * c_coff;
    }

} // namespace Device


#endif // _VMATH_H_
