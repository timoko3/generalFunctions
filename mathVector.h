#ifndef MATH_VECTOR_H
#define MATH_VECTOR_H

#include <array>
#include <cstddef>
#include <iostream>

namespace generalFunctions
{

template <typename T,  std::size_t N>
struct MathVector
{
    std::array<T, N> data; 

    T& operator[]( size_t index) { return data[index]; }
    const T& operator[]( size_t index) const { return data[index]; }

    Vector 
    operator+( const Vector& other) const 
    {
        Vector result;
        for ( size_t i = 0; i < N; ++i ) 
        {
            result.data[i] = this->data[i] + other.data[i];
        }
        return result;
    }

    Vector 
    operator*( T scalar) const {
        Vector result;
        for ( size_t i = 0; i < N; ++i ) 
        {
            result.data[i] = this->data[i] * scalar;
        }
        return result;
    }
};

using Vector2d = MathVector<double, 2>;
using Vector3d = MathVector<double, 3>;

}

#endif /* MATH_VECTOR_H */