#ifndef UTILS_H
#define UTILS_H

#include <random>
#include <type_traits>
#include <stdexcept>
#include <cmath>

/// @brief A 3D vector. The type of the components of this vector is defined by T wich can be: int, long, short, double, float, etc.
template<typename T>
struct vector3 
{ 
    T x{}, y{}, z{}; 

    static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>, "T must be a numeric type.");

    constexpr vector3 operator+(const vector3 &n) const
    {
        vector3 sum;
        sum.x = x + n.x;
        sum.y = y + n.y;
        sum.z = z + n.z;
        return sum;
    }

    constexpr vector3 operator-(const vector3 &n) const
    {
        vector3 diff;
        diff.x = x - n.x;
        diff.y = y - n.y;
        diff.z = z - n.z;
        return diff;
    }

    constexpr vector3 operator*(const T &n) const 
    { 
        vector3 product;
        product.x = x * n;
        product.y = y * n;
        product.z = z * n;
        return product;
    }

    // defines n * v, since the scalar product function above only defined for v * n.
    friend constexpr vector3 operator*(const T &scalar, const vector3 &v) { return v * scalar; }

    constexpr vector3 operator/(const T &n) const 
    { 
        if (n == 0)
            throw std::runtime_error("Math error: Attempted to divide by Zero.");

        vector3 ratio;
        ratio.x = x / n;
        ratio.y = y / n;
        ratio.z = z / n;
        return ratio;
    }

    constexpr T operator*(const vector3 &n) const { return x*n.x + y*n.y + z*n.z; }

    /// @return the euclidien norm of the vector as a double.
    inline double norm() const { return std::sqrt((*this) * (*this)); }

    /// @brief Normalize the vector.
    inline void normalize() 
    { 
        T norm = static_cast<T>(this->norm());
        if (norm == 0)
            throw std::runtime_error("Math error: Zero norm vectors cant be normalized.");

        *this = *this / norm; 
    }
};

inline double uniform01() {
    static std::mt19937 gen(42);
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(gen);
}

/// @brief Return the distance between v0 and v1.
/// @tparam T The vector element data type. 
/// @param v0 the first vector.
/// @param v1 the second vector.
template<typename T>
inline double distance(const vector3<T>& v0, const vector3<T>& v1)
{
    static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>, "T must be a numeric type.");
    return std::sqrt((v1-v0) * (v1-v0));
}

#endif
