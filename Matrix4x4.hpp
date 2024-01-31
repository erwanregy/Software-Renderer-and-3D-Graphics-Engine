#pragma once

#include "Coordinate.hpp"
#include "Vector3D.hpp"

#include <iomanip>


struct Matrix4x4 {
    std::array<double, 4>& operator[](size_t index) { return _elements[index]; }
    const std::array<double, 4>& operator[](size_t index) const { return _elements[index]; }

    Matrix4x4() = default;
    Matrix4x4(const Matrix4x4&) = default;
    explicit Matrix4x4(const std::array<std::array<double, 4>, 4>& elements) : _elements(elements) {}
    Matrix4x4(const std::initializer_list<std::initializer_list<double>>& elements) {
        size_t r = 0;
        for (const auto& row : elements) {
            size_t c = 0;
            for (const auto& element : row) {
                _elements[r][c] = element;
                ++c;
            }
            ++r;
        }
    }

    Matrix4x4& operator=(const Matrix4x4&) = default;
    inline Matrix4x4 operator-() const {
        Matrix4x4 result = *this;
        for (auto& row : result._elements) {
            for (auto& element : row) {
                element = -element;
            }
        }
        return result;
    }


    Matrix4x4& operator+=(const Matrix4x4&);
    Matrix4x4& operator-=(const Matrix4x4&);
    Matrix4x4& operator*=(const Matrix4x4&);

    Matrix4x4& operator*=(double);
    Matrix4x4& operator/=(double);

    Matrix4x4& operator++();
    Matrix4x4& operator--();

    Matrix4x4 operator++(int);
    Matrix4x4 operator--(int);

    Matrix4x4 transpose() const {
        Matrix4x4 result;
        for (size_t row = 0; row < 4; ++row) {
            for (size_t column = 0; column < 4; ++column) {
                result[row][column] = _elements[column][row];
            }
        }
        return result;
    }
    Matrix4x4 sub_matrix(const size_t row, const size_t column) const {
        Matrix4x4 result;
        size_t i = 0;
        for (size_t r = 0; r < 4; ++r) {
            if (r == row) continue;
            size_t j = 0;
            for (size_t c = 0; c < 4; ++c) {
                if (c == column) continue;
                result[i][j] = _elements[r][c];
                ++j;
            }
            ++i;
        }
        return result;
    }
    double determinant() const {
        double result = 0;
        for (size_t i = 0; i < 4; ++i) {
            result += (i % 2 == 0 ? 1 : -1) * _elements[0][i] * sub_matrix(0, i).determinant();
        }
        return result;
    }
    Matrix4x4 inverse() const;
private:
    std::array<std::array<double, 4>, 4> _elements;
};


inline Matrix4x4 operator+(const Matrix4x4& lhs, const Matrix4x4& rhs) {
    Matrix4x4 result;
    for (size_t row = 0; row < 4; ++row) {
        for (size_t column = 0; column < 4; ++column) {
            result[row][column] = lhs[row][column] + rhs[row][column];
        }
    }
    return result;
}
inline Matrix4x4 operator-(const Matrix4x4& lhs, const Matrix4x4& rhs) {
    Matrix4x4 result;
    for (size_t row = 0; row < 4; ++row) {
        for (size_t column = 0; column < 4; ++column) {
            result[row][column] = lhs[row][column] - rhs[row][column];
        }
    }
    return result;
}
inline Matrix4x4 operator*(const Matrix4x4& lhs, const Matrix4x4& rhs) {
    Matrix4x4 result;
    for (size_t row = 0; row < 4; ++row) {
        for (size_t column = 0; column < 4; ++column) {
            result[row][column] = 0;
            for (size_t i = 0; i < 4; ++i) {
                result[row][column] += lhs[row][i] * rhs[i][column];
            }
        }
    }
    return result;
}

inline Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& other) { return *this = *this + other; }
inline Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& other) { return *this = *this - other; }
inline Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& other) { return *this = other * *this; }

inline Matrix4x4 operator*(const Matrix4x4& matrix, const double scalar) {
    Matrix4x4 result = matrix;
    for (size_t row = 0; row < 4; ++row) {
        for (size_t column = 0; column < 4; ++column) {
            result[row][column] *= scalar;
        }
    }
    return result;
}
inline Matrix4x4 operator*(const double scalar, const Matrix4x4& matrix) { return matrix * scalar; }
inline Matrix4x4 operator/(const Matrix4x4& matrix, const double scalar) {
    Matrix4x4 result = matrix;
    for (size_t row = 0; row < 4; ++row) {
        for (size_t column = 0; column < 4; ++column) {
            result[row][column] /= scalar;
        }
    }
    return result;
}

inline Matrix4x4 Matrix4x4::inverse() const {
    Matrix4x4 result;
    double determinant = 0;
    for (size_t i = 0; i < 4; ++i) {
        determinant += (i % 2 == 0 ? 1 : -1) * _elements[0][i] * sub_matrix(0, i).determinant();
    }
    for (size_t row = 0; row < 4; ++row) {
        for (size_t column = 0; column < 4; ++column) {
            result[row][column] = (row + column) % 2 == 0 ? 1 : -1;
            result[row][column] *= sub_matrix(row, column).determinant();
        }
    }
    return result.transpose() / determinant;
}

inline Matrix4x4& Matrix4x4::operator*=(const double scalar) { return *this = *this * scalar; }
inline Matrix4x4& Matrix4x4::operator/=(const double scalar) { return *this = *this / scalar; }

inline Vector3D operator*(const Matrix4x4& matrix, const Vector3D& vector) {
    return {
        matrix[0][0] * vector.x + matrix[0][1] * vector.y + matrix[0][2] * vector.z + matrix[0][3] * vector.w,
        matrix[1][0] * vector.x + matrix[1][1] * vector.y + matrix[1][2] * vector.z + matrix[1][3] * vector.w,
        matrix[2][0] * vector.x + matrix[2][1] * vector.y + matrix[2][2] * vector.z + matrix[2][3] * vector.w,
        matrix[3][0] * vector.x + matrix[3][1] * vector.y + matrix[3][2] * vector.z + matrix[3][3] * vector.w
    };
};
inline Vector3D operator*(const Vector3D& vector, const Matrix4x4& matrix) { return matrix * vector; }

inline Vector3D& operator*=(Vector3D& vector, const Matrix4x4& matrix) { return vector = matrix * vector; }

inline Matrix4x4 make_identity_matrix() {
    return {
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 }
    };
}

inline Matrix4x4& Matrix4x4::operator++() { return *this += make_identity_matrix(); }
inline Matrix4x4& Matrix4x4::operator--() { return *this -= make_identity_matrix(); }

inline Matrix4x4 Matrix4x4::operator++(int) { const Matrix4x4 result = *this; ++*this; return result; }
inline Matrix4x4 Matrix4x4::operator--(int) { const Matrix4x4 result = *this; --*this; return result; }

inline Matrix4x4 make_translation_matrix(const double x, const double y, const double z) {
    return {
        { 1, 0, 0, x },
        { 0, 1, 0, y },
        { 0, 0, 1, z },
        { 0, 0, 0, 1 }
    };
}
inline Matrix4x4 make_translation_matrix(const Vector3D& axes) { return make_translation_matrix(axes.x, axes.y, axes.z); }

inline Matrix4x4 make_scaling_matrix(const double x, const double y, const double z) {
    return {
        { x, 0, 0, 0 },
        { 0, y, 0, 0 },
        { 0, 0, z, 0 },
        { 0, 0, 0, 1 }
    };
}
inline Matrix4x4 make_scaling_matrix(const Vector3D& axes) { return make_scaling_matrix(axes.x, axes.y, axes.z); }

inline Matrix4x4 make_rotation_matrix_x(const double angle_radians) {
    return {
        { 1, 0,                       0,                        0 },
        { 0, std::cos(angle_radians), -std::sin(angle_radians), 0 },
        { 0, std::sin(angle_radians), std::cos(angle_radians),  0 },
        { 0, 0,                       0,                        1 }
    };
}
inline Matrix4x4 make_rotation_matrix_y(const double angle_radians) {
    return {
        { std::cos(angle_radians),  0, std::sin(angle_radians), 0 },
        { 0,                        1, 0,                       0 },
        { -std::sin(angle_radians), 0, std::cos(angle_radians), 0 },
        { 0,                        0, 0,                       1 }
    };
}
inline Matrix4x4 make_rotation_matrix_z(const double angle_radians) {
    return {
        { std::cos(angle_radians), -std::sin(angle_radians), 0, 0 },
        { std::sin(angle_radians), std::cos(angle_radians),  0, 0 },
        { 0,                       0,                        1, 0 },
        { 0,                       0,                        0, 1 }
    };
}

inline Matrix4x4 make_rotation_matrix(const double x, const double y, const double z) {
    return {
        { std::cos(x) * std::cos(y), std::cos(x) * std::sin(y) * std::sin(z) - std::sin(x) * std::cos(z), std::cos(x) * std::sin(y) * std::cos(z) + std::sin(x) * std::sin(z), 0 },
        { std::sin(x) * std::cos(y), std::sin(x) * std::sin(y) * std::sin(z) + std::cos(x) * std::cos(z), std::sin(x) * std::sin(y) * std::cos(z) - std::cos(x) * std::sin(z), 0 },
        { -std::sin(y),              std::cos(y) * std::sin(z),                                           std::cos(y) * std::cos(z),                                           0 },
        { 0,                         0,                                                                   0,                                                                   1 }
    };
}
inline Matrix4x4 make_rotation_matrix(const Vector3D& angles_radians) {
    return make_rotation_matrix(angles_radians.x, angles_radians.y, angles_radians.z);
}

inline Matrix4x4 make_projection_matrix(const Coordinate& screen_dimensions, const double fov_degrees, const double z_near, const double z_far) {
    const double aspect_ratio = static_cast<double>(screen_dimensions.x) / static_cast<double>(screen_dimensions.y);
    const double fov_radians = fov_degrees * 3.14159265358979323846 / 180;
    const double y_scale = 1 / std::tan(fov_radians / 2);
    const double x_scale = y_scale / aspect_ratio;
    const double z_range = z_far - z_near;
    const double z_scale = z_far / z_range;
    const double z_translation = -z_near * z_scale;
    return {
        { x_scale, 0,     0,       0           },
        { 0,     y_scale, 0,       0           },
        { 0,     0,       z_scale, z_translation },
        { 0,     0,       -1,      0           }
    };
}

inline Matrix4x4 make_camera_matrix(const Vector3D& position, const Vector3D& target, const Vector3D& up = { 0, 1, 0 }) {
    const Vector3D& new_forward = (target - position).normalise();
    const Vector3D& new_up = (up - new_forward * dot(up, new_forward)).normalise();
    const Vector3D& new_right = cross(new_forward, new_up);
    return {
        { new_right.x, new_up.x, new_forward.x, position.x },
        { new_right.y, new_up.y, new_forward.y, position.y },
        { new_right.z, new_up.z, new_forward.z, position.z },
        { 0,           0,        0,             1        }
    };
}

inline Matrix4x4 quick_inverse(const Matrix4x4& matrix) {
    return {
        { matrix[0][0], matrix[1][0], matrix[2][0], 0 },
        { matrix[0][1], matrix[1][1], matrix[2][1], 0 },
        { matrix[0][2], matrix[1][2], matrix[2][2], 0 },
        { -dot(matrix[0], matrix[3]), -dot(matrix[1], matrix[3]), -dot(matrix[2], matrix[3]), 1  }
    };
}

inline Matrix4x4 make_view_matrix(const Vector3D& position, const Vector3D& target, const Vector3D& up = { 0, 1, 0 }) {
    const Vector3D& new_forward = (target - position).normalise();
    const Vector3D& new_up = (up - new_forward * dot(up, new_forward)).normalise();
    const Vector3D& new_right = cross(new_forward, new_up);
    return {
        { new_right.x,   new_right.y,   new_right.z,   -dot(position, new_right)   },
        { new_up.x,      new_up.y,      new_up.z,      -dot(position, new_up)      },
        { new_forward.x, new_forward.y, new_forward.z, -dot(position, new_forward) },
        { 0,             0,             0,             1                         }
    };
}

inline std::ostream& operator<<(std::ostream& stream, const Matrix4x4& matrix) {
    for (size_t row = 0; row < 4; ++row) {
        stream << "[ ";
        for (size_t column = 0; column < 4; ++column) {
            stream << std::setw(5) << std::fixed << std::setprecision(2) << matrix[row][column] << ' ';
        }
        stream << "]\n";
    }
    return stream;
}

//inline bool operator==(const Matrix4x4&, const Matrix4x4&);
//inline bool operator!=(const Matrix4x4&, const Matrix4x4&);
//inline bool operator<(const Matrix4x4&, const Matrix4x4&);
//inline bool operator>(const Matrix4x4&, const Matrix4x4&);
//inline bool operator<=(const Matrix4x4&, const Matrix4x4&);
//inline bool operator>=(const Matrix4x4&, const Matrix4x4&);