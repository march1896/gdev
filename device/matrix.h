#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <cmath>
#include <cassert>
#include <utility>
#include <iostream>
#include <iomanip>

#include "prelude.h"
#include "vector.h"

namespace Device {

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
    void rotate_x(Matrix44<T>& mat, float angle)
    {
        // TODO:
    }

    template <typename T>
    void rotate_y(Matrix44<T>& mat, float angle)
    {
        // TODO:
    }

    template <typename T>
    void rotate_z(Matrix44<T>& mat, float angle)
    {
        // TODO:
    }

    template <typename T>
    void scale(Matrix44<T>& mat, float x, float y, float z)
    {
        // TODO:
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

} // namespace Device
#endif // _MATRIX_H_
