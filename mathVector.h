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

    MathVector 
    operator+( const MathVector& other) const 
    {
        MathVector result;
        for ( size_t i = 0; i < N; ++i ) 
        {
            result.data[i] = this->data[i] + other.data[i];
        }
        return result;
    }

    MathVector 
    operator*( T scalar) const {
        MathVector result;
        for ( size_t i = 0; i < N; ++i ) 
        {
            result.data[i] = this->data[i] * scalar;
        }
        return result;
    }

    MathVector& 
    operator+=(const MathVector& other)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            data[i] += other.data[i];
        }

        return *this;
    }
};

template <typename T, std::size_t N>
std::ostream& operator<<( std::ostream& os, const MathVector<T, N>& vec)
{
    os << "(";

    for ( std::size_t i = 0; i < N; ++i )
    {
        os << vec.data[i];

        if ( i + 1 < N )
            os << ", ";
    }

    os << ")";

    return os;
}

using Vector2d = MathVector<double, 2>;
using Vector3d = MathVector<double, 3>;

}

#endif /* MATH_VECTOR_H */