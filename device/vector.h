#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <cmath>
#include <iostream>
#include <iomanip>

#include "prelude.h"

namespace Device {
    template <typename T>
    struct Vec2
    {
        T x, y;
    };

    template <>
    struct Vec2<float>
    {
        float x, y;

        inline float length() const
        {
            return std::sqrt(x * x + y * y);
        }

        inline void normalize()
        {
            float const len = length();
            x /= len;
            y /= len;
        }
    };

    typedef Vec2<float> Vec2f;
    typedef Vec2<int> Vec2i;

    inline Vec2f normalize(Vec2f const& v)
    {
        float const len = v.length();
        return Vec2f{v.x/len, v.y/len};
    }

    template <typename T>
    inline Vec2<T> operator+ (Vec2<T> const& a, Vec2<T> const& b)
    {
        return Vec2<T>{a.x + b.x, a.y + b.y};
    }

    template <typename T>
    inline Vec2<T> operator+ (Vec2<T> const& a, T const& m)
    {
        return Vec2<T>{a.x + m, a.y + m};
    }

    template <typename T>
    inline Vec2<T> operator+ (T const& m, Vec2<T> const& a)
    {
        return Vec2<T>{a.x + m, a.y + m};
    }

    template <typename T>
    inline Vec2<T> operator- (Vec2<T> const& a, Vec2<T> const& b)
    {
        return Vec2<T>{a.x - b.x, a.y - b.y};
    }

    template <typename T>
    inline Vec2<T> operator- (Vec2<T> const& a, T const& m)
    {
        return Vec2<T>{a.x - m, a.y - m};
    }

    template <typename T>
    inline Vec2<T> operator- (T const& m, Vec2<T> const& a)
    {
        return Vec2<T>{m - a.x, m - a.y};
    }

    template <typename T>
    inline Vec2<T> operator* (Vec2<T> const& a, T const& m)
    {
        return Vec2<T>{a.x * m, a.y * m};
    }

    template <typename T>
    inline Vec2<T> operator* (T const& m, Vec2<T> const& a)
    {
        return Vec2<T>{a.x * m, a.y * m};
    }

    // [ref](https://en.wikibooks.org/wiki/GLSL_Programming/Vector_and_Matrix_Operations#Operators)
    template <typename T>
    inline Vec2<T> operator* (Vec2<T> const& a, Vec2<T> const& b)
    {
        return Vec2<T>{a.x * b.x, a.y * b.y};
    }

    template <typename T>
    inline Vec2<T> operator/ (Vec2<T> const& a, Vec2<T> const& b)
    {
        return Vec2<T>{a.x / b.x, a.y / b.y, a.z / b.z};
    }

    template <typename T>
    inline T dot(Vec2<T> const& a, Vec2<T> const& b)
    {
        return a.x * b.x + a.y * b.y;
    }

    template <typename T>
    inline float cross(Vec2<T> const& a, Vec2<T> const& b)
    {
        return a.x * b.y - a.y * b.x;
    }
    ///////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct Vec3
    {
        T x, y, z;
    };

    template <>
    struct Vec3<float>
    {
        float x, y, z;

        inline float length() const
        {
            return std::sqrt(x * x + y * y + z * z);
        }

        inline void normalize()
        {
            float const len = length();
            x /= len;
            y /= len;
            z /= len;
        }
    };

    typedef Vec3<float> Vec3f;
    typedef Vec3<int> Vec3i;

    inline Vec3f normalize(Vec3f const& v)
    {
        float const len = v.length();
        return Vec3f{v.x/len, v.y/len, v.z/len};
    }

    template <typename T>
    inline Vec3<T> operator+ (Vec3<T> const& a, Vec3<T> const& b)
    {
        return Vec3<T>{a.x + b.x, a.y + b.y, a.z + b.z};
    }

    template <typename T>
    inline Vec3<T> operator+ (Vec3<T> const& a, T const& m)
    {
        return Vec3<T>{a.x + m, a.y + m, a.z + m};
    }

    template <typename T>
    inline Vec3<T> operator+ (T const& m, Vec3<T> const& a)
    {
        return Vec3<T>{a.x + m, a.y + m, a.z + m};
    }

    template <typename T>
    inline Vec3<T> operator- (Vec3<T> const& a, Vec3<T> const& b)
    {
        return Vec3<T>{a.x + b.x, a.y + b.y, a.z - b.z};
    }

    template <typename T>
    inline Vec3<T> operator- (Vec3<T> const& a, T const& m)
    {
        return Vec3<T>{a.x - m, a.y - m, a.z - m};
    }

    template <typename T>
    inline Vec3<T> operator- (T const& m, Vec3<T> const& a)
    {
        return Vec3<T>{m - a.x, m - a.y, m - a.z};
    }

    template <typename T>
    inline Vec3<T> operator* (Vec3<T> const& a, T const& m)
    {
        return Vec3<T>{a.x * m, a.y * m, a.z * m};
    }

    template <typename T>
    inline Vec3<T> operator* (T const& m, Vec3<T> const& a)
    {
        return Vec3<T>{a.x * m, a.y * m, a.z * m};
    }

    // Note: operator* operator/ works on component-wise.
    // [ref](https://en.wikibooks.org/wiki/GLSL_Programming/Vector_and_Matrix_Operations#Operators)
    template <typename T>
    inline Vec3<T> operator* (Vec3<T> const& a, Vec3<T> const& b)
    {
        return Vec3<T>{a.x * b.x, a.y * b.y, a.z * b.z};
    }

    template <typename T>
    inline Vec3<T> operator/ (Vec3<T> const& a, Vec3<T> const& b)
    {
        return Vec3<T>{a.x / b.x, a.y / b.y, a.z / b.z};
    }

    template <typename T>
    inline Vec3<T> operator/ (Vec3<T> const& a, T const& m)
    {
        return Vec3<T>{a.x / m, a.y / m, a.z / m};
    }

    template <typename T>
    inline T dot(Vec3<T> const& a, Vec3<T> const& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    template <typename T>
    inline Vec3<T> cross(Vec3<T> const& a, Vec3<T> const& b)
    {
        return Vec3<T>{
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct Vec4
    {
        T x, y, z, w;
    };


    typedef Vec4<float> Vec4f;
    typedef Vec4<int> Vec4i;

    template <typename T>
    inline Vec4<T> operator+ (Vec4<T> const& a, Vec4<T> const& b)
    {
        return Vec4<T>{a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
    }

    template <typename T>
    inline Vec4<T> operator+ (Vec4<T> const& a, T const& m)
    {
        return Vec4<T>{a.x + m, a.y + m, a.z + m, a.w + m};
    }

    template <typename T>
    inline Vec4<T> operator+ (T const& m, Vec4<T> const& a)
    {
        return Vec4<T>{a.x + m, a.y + m, a.z + m, a.w + m};
    }

    template <typename T>
    inline Vec4<T> operator- (Vec4<T> const& a, Vec4<T> const& b)
    {
        return Vec4<T>{a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
    }

    template <typename T>
    inline Vec4<T> operator- (Vec4<T> const& a, T const& m)
    {
        return Vec4<T>{a.x - m, a.y - m, a.z - m, a.w - m};
    }

    template <typename T>
    inline Vec4<T> operator- (T const& m, Vec4<T> const& a)
    {
        return Vec4<T>{m - a.x, m - a.y, m - a.z, m - a.w};
    }

    template <typename T>
    inline Vec4<T> operator* (Vec4<T> const& a, T const& m)
    {
        return Vec4<T>{a.x * m, a.y * m, a.z * m, a.w * m};
    }

    template <typename T>
    inline Vec4<T> operator* (T const& m, Vec4<T> const& a)
    {
        return Vec4<T>{m * a.x, m * a.y, m * a.z, m * a.w};
    }

    template <typename T>
    inline Vec4<T> operator/ (Vec4<T> const& a, T const& m)
    {
        return Vec4<T>{a.x / m, a.y / m, a.z / m, a.w / m};
    }

    template <typename T>
    inline Vec4<T> operator/ (T const& m, Vec4<T> const& a)
    {
        return Vec4<T>{m / a.x, m / a.y, m / a.z, m / a.w};
    }

    // [ref](https://en.wikibooks.org/wiki/GLSL_Programming/Vector_and_Matrix_Operations#Operators)
    template <typename T>
    inline Vec4<T> operator* (Vec4<T> const& a, Vec4<T> const& b)
    {
        return Vec4<T>{a.x * b.x, a.y * b.y};
    }

    template <typename T>
    inline Vec4<T> operator/ (Vec4<T> const& a, Vec4<T> const& b)
    {
        return Vec4<T>{a.x / b.x, a.y / b.y, a.z / b.z};
    }

    template <typename T>
    inline T dot(Vec4<T> const& a, Vec4<T> const& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    template <typename T>
    std::ostream & operator<<(std::ostream& stream, Vec2<T> const& vec) {
        stream << "Vec2{" << vec.x << ", " << vec.y << "}";
        return stream;
    }

    template <typename T>
    std::ostream & operator<<(std::ostream& stream, Vec3<T> const& vec) {
        stream << "Vec3{" << vec.x << ", " << vec.y << ", " << vec.z << "}";
        return stream;
    }

    template <typename T>
    std::ostream & operator<<(std::ostream& stream, Vec4<T> const& vec) {
        stream << "Vec4{" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << "}";
        return stream;
    }

} // namespace Device

#endif // _VECTOR_H_
