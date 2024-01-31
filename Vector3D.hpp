#pragma once

#include <array>
#include <vector>


struct Vector3D {
    double x, y, z, w;

    Vector3D(const double x = 0, const double y = 0, const double z = 0, const double w = 1) : x(x), y(y), z(z), w(w) {}
    Vector3D(const std::initializer_list<double>& list) : x(*list.begin()), y(*(list.begin() + 1)), z(*(list.begin() + 2)), w(*(list.begin() + 3)) {}
    Vector3D(const std::vector<double>& vector) : x(vector[0]), y(vector[1]), z(vector[2]), w(1) { if (vector.size() == 4) w = vector[3]; }
    Vector3D(const std::array<double, 3>& array) : x(array[0]), y(array[1]), z(array[2]), w(1) {}
    Vector3D(const std::array<double, 4>& array) : x(array[0]), y(array[1]), z(array[2]), w(array[3]) {}
    Vector3D(const Vector3D&) = default;

    Vector3D& operator=(const Vector3D&) = default;
    Vector3D operator-() const { return { -x, -y, -z, -w }; }

    double magnitude() const;
    Vector3D& normalise();
    Vector3D normalised() const;

    Vector3D& operator+=(const Vector3D& other);
    Vector3D& operator-=(const Vector3D& other);

    Vector3D& operator*=(double scalar);
    Vector3D& operator/=(double scalar);

    Vector3D& operator++() { return *this += { 1, 1, 1 }; }
    Vector3D& operator--() { return *this -= { 1, 1, 1 }; }

    Vector3D operator++(int) { const Vector3D temp = *this; ++*this; return temp; }
    Vector3D operator--(int) { const Vector3D temp = *this; --*this; return temp; }
};


inline double dot(const Vector3D& lhs, const Vector3D& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
inline Vector3D cross(const Vector3D& lhs, const Vector3D& rhs) {
    return {
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    };
}

inline double Vector3D::magnitude() const { return sqrt(dot(*this, *this)); }

inline Vector3D operator+(const Vector3D& lhs, const Vector3D& rhs) { return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z }; }
inline Vector3D operator-(const Vector3D& lhs, const Vector3D& rhs) { return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z }; }

inline Vector3D& Vector3D::operator+=(const Vector3D& other) { return *this = *this + other; }
inline Vector3D& Vector3D::operator-=(const Vector3D& other) { return *this = *this - other; }

inline Vector3D operator*(const Vector3D& vector, const double scalar) { return { vector.x * scalar, vector.y * scalar, vector.z * scalar }; }
inline Vector3D operator*(const double scalar, const Vector3D& vector) { return vector * scalar; }
inline Vector3D operator/(const Vector3D& vector, const double scalar) { return { vector.x / scalar, vector.y / scalar, vector.z / scalar }; }

inline Vector3D& Vector3D::operator*=(const double scalar) { return *this = *this * scalar; }
inline Vector3D& Vector3D::operator/=(const double scalar) { return *this = *this / scalar; }

inline Vector3D& Vector3D::normalise() { return *this /= magnitude(); }
inline Vector3D Vector3D::normalised() const { return *this / magnitude(); }