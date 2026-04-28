/// @file utils.h
/// @brief Utility types and functions shared across the simulation.
///
/// Provides:
/// - @c vector3<T>: a generic 3-D vector with arithmetic operators, norm, and normalisation.
/// - @c uniform01(): a convenience function returning a uniform random double in [0, 1).
/// - @c distance(): Euclidean distance between two @c vector3 values.

#pragma once

#include <random>
#include <type_traits>
#include <stdexcept>
#include <cmath>
#include<fstream>

/// @brief A generic 3-D vector whose component type is @p T.
///
/// @tparam T Component type. Must be an integral or floating-point type
///           (enforced via @c static_assert).
///
/// Supports element-wise addition, subtraction, scalar multiplication/division,
/// dot product, Euclidean norm, normalisation, equality comparison, and stream output.
template<typename T>
struct vector3 
{ 
    T x{}, y{}, z{}; 

    static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>, "T must be a numeric type.");

    /// @brief Element-wise addition.
    /// @param n Right-hand-side vector.
    /// @return Component-wise sum of @c *this and @p n.
    constexpr vector3 operator+(const vector3 &n) const
    {
        vector3 sum;
        sum.x = x + n.x;
        sum.y = y + n.y;
        sum.z = z + n.z;
        return sum;
    }

    /// @brief In-place element-wise addition.
    /// @param n Right-hand-side vector.
    constexpr void operator+=(const vector3 &n) { (*this) = (*this) + n; }

    /// @brief Element-wise subtraction.
    /// @param n Right-hand-side vector.
    /// @return Component-wise difference @c *this - @p n.
    constexpr vector3 operator-(const vector3 &n) const
    {
        vector3 diff;
        diff.x = x - n.x;
        diff.y = y - n.y;
        diff.z = z - n.z;
        return diff;
    }

    /// @brief In-place element-wise subtraction.
    /// @param n Right-hand-side vector.
    constexpr void operator-=(const vector3 &n) { (*this) = (*this) - n; }

    /// @brief Scalar multiplication (vector * scalar).
    /// @param n Scalar multiplier.
    /// @return Each component of @c *this multiplied by @p n.
    constexpr vector3 operator*(const T &n) const 
    { 
        vector3 product;
        product.x = x * n;
        product.y = y * n;
        product.z = z * n;
        return product;
    }

    // defines n * v, since the scalar product function above only defined for v * n.
    /// @brief Scalar multiplication (scalar * vector) — commutative companion to @c operator*(T).
    /// @param scalar Scalar multiplier.
    /// @param v      Vector operand.
    /// @return Each component of @p v multiplied by @p scalar.
    /// @category wizardry
    friend constexpr vector3 operator*(const T &scalar, const vector3 &v) { return v * scalar; }

    /// @brief In-place scalar multiplication.
    /// @param n Scalar multiplier.
    constexpr void operator*=(const T &n) { (*this) = (*this) * n; }

    /// @brief Dot product.
    /// @param n Right-hand-side vector.
    /// @return Scalar dot product @c x*n.x + y*n.y + z*n.z.
    constexpr T operator*(const vector3 &n) const { return x*n.x + y*n.y + z*n.z; }

    /// @brief In-place component-wise multiplication (replaces each component with the dot product).
    ///        @note Due to the implementation delegating to the scalar dot-product @c operator*(vector3),
    ///              the resulting assignment of a scalar @p T to a @c vector3 is unusual and may not
    ///              compile for all types. Prefer explicit dot-product usage where needed.
    /// @param n Right-hand-side vector.
    constexpr void operator*=(const vector3 &n) { (*this) = (*this) * n; }

    /// @brief Scalar division.
    /// @param n Scalar divisor. Must not be zero.
    /// @return Each component of @c *this divided by @p n.
    /// @throws std::runtime_error if @p n is zero.
    constexpr vector3 operator/(const T &n) const 
    {
        if (n == 0)
            throw std::runtime_error("Vector Math error: Attempted to divide by Zero.");

        vector3 ratio;
        ratio.x = x / n;
        ratio.y = y / n;
        ratio.z = z / n;
        return ratio;
    }

    /// @brief In-place scalar division.
    ///        Divides each component by @p n and stores the result in @c *this.
    ///        @note The declared return type is @c vector3 but the function body does not include
    ///              an explicit @c return statement; treat this as an in-place mutation only.
    /// @param n Scalar divisor.
    constexpr vector3 operator/=(const T &n) { (*this) = (*this) / n; }

    /// @brief Equality comparison.
    /// @param n Right-hand-side vector.
    /// @return @c true if all three components are equal.
    constexpr bool operator==(const vector3 &n) const { return x == n.x && y == n.y && z == n.z; }


    /// @brief Overload the << operator to print the vector in the format (x, y, z).
    friend std::ostream& operator<<(std::ostream& os, const vector3& vec) 
    {
        os << "(" << vec.x << "," << vec.y << "," << vec.z << ")";
        return os;
    }

    /// @return the euclidien norm of the vector as a double.
    inline double norm() const { return std::sqrt((*this) * (*this)); }

    /// @brief Normalize the vector.
    inline void normalize() 
    { 
        T norm = static_cast<T>(this->norm());
        if (norm == 0)
            throw std::runtime_error("Vector Math error: Zero norm vectors cant be normalized.");

        *this = *this / norm; 
    }
};

/// @brief Return a random double in the range [0, 1) using a Mersenne Twister random number generator.
inline double uniform01() 
{
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
