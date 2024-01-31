#pragma once

#include "Vector3D.hpp"

#include "Matrix4x4.hpp"

#include <array>


struct Triangle {
    std::array<Vector3D, 3> vertices;
    double illumination = 0;

    Triangle() = default;
    Triangle(std::array<Vector3D, 3> vertices) : vertices(vertices) {}
    Triangle(std::initializer_list<Vector3D> vertices) {
        size_t i = 0;
        for (auto& vertex : vertices) {
            this->vertices[i++] = vertex;
        }
    }

    Vector3D normal() const { return cross(vertices[1] - vertices[0], vertices[2] - vertices[0]).normalise(); }
    double depth() const { return (vertices[0].z + vertices[1].z + vertices[2].z) / 3; }

    void transform(const Matrix4x4& matrix) {
        for (auto& vertex : vertices) {
            vertex *= matrix;
        }
    }
};