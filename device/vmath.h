#ifndef _VMATH_H_
#define _VMATH_H_

typedef unsigned int U32;
typedef unsigned short U16;
typedef unsigned char U8;

typedef int I32;
typedef short I16;
typedef char I8;

// TODO: move this to some utilities
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

#include <cmath>
#include <cassert>

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

#include <utility>
template <typename T>
struct Matrix44
{
    static constexpr U32 size = 4;

    struct Row {
        T _data[4];
        T& operator[](U32 idx) { return _data[idx]; }
        const T& operator[](U32 idx) const { return _data[idx]; }
    };

    union {
        Row _rows[4];
        T _data[4][4];
    };

    inline Row& operator[](U32 idx)
    {
        return _rows[idx];
    }

    inline const Row& operator[](U32 idx) const
    {
        return _rows[idx];
    }

    inline void make_identity()
    {
        for (U32 i = 0; i < size; ++i)
        {
            for (U32 j = 0; j < size; ++j)
            {
                if (i == j)
                    _data[i][j] = 1;
                else
                    _data[i][j] = 0;
            }
        }
    }

    inline void transpose()
    {
        for (U32 i = 0; i < size - 1; ++i)
        {
            for (U32 j = i + 1; j < size; ++j)
            {
                std::swap(_data[i][j], _data[j][i]);
            }
        }
    }
};

typedef Matrix44<float> Mat44f;

template <typename T>
Matrix44<T> transpose(Matrix44<T> const& m)
{
    return Matrix44<T> {
        m[0][0], m[1][0], m[2][0], m[3][0],
        m[0][1], m[1][1], m[2][1], m[3][1],
        m[0][2], m[1][2], m[2][2], m[3][2],
        m[0][3], m[1][3], m[2][3], m[3][3],
    };
}

template <typename T>
void transform(Matrix44<T>& mat, Vec3f dir)
{
    assert(mat[3][3] == T(1));
    mat[0][3] += dir.x;
    mat[1][3] += dir.y;
    mat[2][3] += dir.z;
}


template <typename T>
Matrix44<T> operator+ (Matrix44<T> const& a, Matrix44<T> const& b)
{
    return Matrix44<T>{
        a[0][0] + b[0][0], a[0][1] + b[0][1], a[0][2] + b[0][2], a[0][3] + b[0][3],
        a[1][0] + b[1][0], a[1][1] + b[1][1], a[1][2] + b[1][2], a[1][3] + b[1][3],
        a[2][0] + b[2][0], a[2][1] + b[2][1], a[2][2] + b[2][2], a[2][3] + b[2][3],
        a[3][0] + b[3][0], a[3][1] + b[3][1], a[3][2] + b[3][2], a[3][3] + b[3][3],
    };
}

template <typename T>
Matrix44<T> operator+ (Matrix44<T> const& a, T const& t)
{
    return Matrix44<T>{
        a[0][0] + t, a[0][1] + t, a[0][2] + t, a[0][3] + t,
        a[1][0] + t, a[1][1] + t, a[1][2] + t, a[1][3] + t,
        a[2][0] + t, a[2][1] + t, a[2][2] + t, a[2][3] + t,
        a[3][0] + t, a[3][1] + t, a[3][2] + t, a[3][3] + t,
    };
}

template <typename T>
Matrix44<T> operator+ (T const& t, Matrix44<T> const& b)
{
    return Matrix44<T>{
        t + b[0][0], t + b[0][1], t + b[0][2], t + b[0][3],
        t + b[1][0], t + b[1][1], t + b[1][2], t + b[1][3],
        t + b[2][0], t + b[2][1], t + b[2][2], t + b[2][3],
        t + b[3][0], t + b[3][1], t + b[3][2], t + b[3][3],
    };
}

template <typename T>
Matrix44<T> operator- (Matrix44<T> const& a, Matrix44<T> const& b)
{
    return Matrix44<T>{
        a[0][0] - b[0][0], a[0][1] - b[0][1], a[0][2] - b[0][2], a[0][3] - b[0][3],
        a[1][0] - b[1][0], a[1][1] - b[1][1], a[1][2] - b[1][2], a[1][3] - b[1][3],
        a[2][0] - b[2][0], a[2][1] - b[2][1], a[2][2] - b[2][2], a[2][3] - b[2][3],
        a[3][0] - b[3][0], a[3][1] - b[3][1], a[3][2] - b[3][2], a[3][3] - b[3][3],
    };
}

template <typename T>
Matrix44<T> operator- (Matrix44<T> const& a, T const& t)
{
    return Matrix44<T>{
        a[0][0] - t, a[0][1] - t, a[0][2] - t, a[0][3] - t,
        a[1][0] - t, a[1][1] - t, a[1][2] - t, a[1][3] - t,
        a[2][0] - t, a[2][1] - t, a[2][2] - t, a[2][3] - t,
        a[3][0] - t, a[3][1] - t, a[3][2] - t, a[3][3] - t,
    };
}

template <typename T>
Matrix44<T> operator- (T const& t, Matrix44<T> const& b)
{
    return Matrix44<T>{
        t - b[0][0], t - b[0][1], t - b[0][2], t - b[0][3],
        t - b[1][0], t - b[1][1], t - b[1][2], t - b[1][3],
        t - b[2][0], t - b[2][1], t - b[2][2], t - b[2][3],
        t - b[3][0], t - b[3][1], t - b[3][2], t - b[3][3],
    };
}

template <typename T>
Matrix44<T> operator* (Matrix44<T> const& a, Matrix44<T> const& b)
{
    return Matrix44<T>{
        a[0][0] * b[0][0] + a[0][1] * b[1][0] + a[0][2] * b[2][0] + a[0][3] * b[3][0],
        a[0][0] * b[0][1] + a[0][1] * b[1][1] + a[0][2] * b[2][1] + a[0][3] * b[3][1],
        a[0][0] * b[0][2] + a[0][1] * b[1][2] + a[0][2] * b[2][2] + a[0][3] * b[3][2],
        a[0][0] * b[0][3] + a[0][1] * b[1][3] + a[0][2] * b[2][3] + a[0][3] * b[3][3],

        a[1][0] * b[0][0] + a[1][1] * b[1][0] + a[1][2] * b[2][0] + a[1][3] * b[3][0],
        a[1][0] * b[0][1] + a[1][1] * b[1][1] + a[1][2] * b[2][1] + a[1][3] * b[3][1],
        a[1][0] * b[0][2] + a[1][1] * b[1][2] + a[1][2] * b[2][2] + a[1][3] * b[3][2],
        a[1][0] * b[0][3] + a[1][1] * b[1][3] + a[1][2] * b[2][3] + a[1][3] * b[3][3],

        a[2][0] * b[0][0] + a[2][1] * b[1][0] + a[2][2] * b[2][0] + a[2][3] * b[3][0],
        a[2][0] * b[0][1] + a[2][1] * b[1][1] + a[2][2] * b[2][1] + a[2][3] * b[3][1],
        a[2][0] * b[0][2] + a[2][1] * b[1][2] + a[2][2] * b[2][2] + a[2][3] * b[3][2],
        a[2][0] * b[0][3] + a[2][1] * b[1][3] + a[2][2] * b[2][3] + a[2][3] * b[3][3],

        a[3][0] * b[0][0] + a[3][1] * b[1][0] + a[3][2] * b[2][0] + a[3][3] * b[3][0],
        a[3][0] * b[0][1] + a[3][1] * b[1][1] + a[3][2] * b[2][1] + a[3][3] * b[3][1],
        a[3][0] * b[0][2] + a[3][1] * b[1][2] + a[3][2] * b[2][2] + a[3][3] * b[3][2],
        a[3][0] * b[0][3] + a[3][1] * b[1][3] + a[3][2] * b[2][3] + a[3][3] * b[3][3],
    };
}

template <typename T>
Vec4<T> operator* (Matrix44<T> const& m, Vec4<T> const& v)
{
    return Vec4<T>{
        m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
        m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
        m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
        m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w,
    };
}

template <typename T>
Matrix44<T> operator* (Matrix44<T> const& a, T const& t)
{
    return Matrix44<T>{
        a[0][0] * t, a[0][1] * t, a[0][2] * t, a[0][3] * t,
        a[1][0] * t, a[1][1] * t, a[1][2] * t, a[1][3] * t,
        a[2][0] * t, a[2][1] * t, a[2][2] * t, a[2][3] * t,
        a[3][0] * t, a[3][1] * t, a[3][2] * t, a[3][3] * t,
    };
}

template <typename T>
Matrix44<T> operator* (T const& t, Matrix44<T> const& b)
{
    return Matrix44<T>{
        t * b[0][0], t * b[0][1], t * b[0][2], t * b[0][3],
        t * b[1][0], t * b[1][1], t * b[1][2], t * b[1][3],
        t * b[2][0], t * b[2][1], t * b[2][2], t * b[2][3],
        t * b[3][0], t * b[3][1], t * b[3][2], t * b[3][3],
    };
}

template <typename T>
Matrix44<T> operator/ (Matrix44<T> const& a, T const& t)
{
    return Matrix44<T>{
        a[0][0] / t, a[0][1] / t, a[0][2] / t, a[0][3] / t,
        a[1][0] / t, a[1][1] / t, a[1][2] / t, a[1][3] / t,
        a[2][0] / t, a[2][1] / t, a[2][2] / t, a[2][3] / t,
        a[3][0] / t, a[3][1] / t, a[3][2] / t, a[3][3] / t,
    };
}

template <typename T>
Matrix44<T> operator/ (T const& t, Matrix44<T> const& b)
{
    return Matrix44<T>{
        t / b[0][0], t / b[0][1], t / b[0][2], t / b[0][3],
        t / b[1][0], t / b[1][1], t / b[1][2], t / b[1][3],
        t / b[2][0], t / b[2][1], t / b[2][2], t / b[2][3],
        t / b[3][0], t / b[3][1], t / b[3][2], t / b[3][3],
    };
}

// TODO: calculate the inverse matrix

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

#include <iostream>
#include <iomanip>
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

template <typename T>
std::ostream & operator<<(std::ostream& stream, Matrix44<T> const& mat) {
    stream << std::fixed << std::setw(11) << std::setprecision(6);
    stream << "Matrix44 {" << std::endl;
    for (U32 i = 0; i < 4; ++i)
    {
        for (U32 j = 0; j < 4; ++j)
        {
            stream << mat[i][j];
            if (j != 3) stream << ", ";
            if (j == 3 && i != 3) stream << std::endl;
        }
        if (i == 3) stream << "}";
    }
    return stream;
}

#endif // _VMATH_H_
