#pragma once

#include "Triangle.hpp"

#include "Matrix4x4.hpp"

#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>


struct Mesh {
    std::vector<Triangle> triangles;

    Mesh() = default;

    Mesh(const std::string& filename) {
        std::ifstream file_stream(filename);
        if (not file_stream.is_open()) {
            throw std::runtime_error("Could not open file " + filename);
        }

        std::vector<Vector3D> vertices;

        size_t line_number = 1;

        while (not file_stream.eof()) {
            std::string line;
            std::getline(file_stream, line);
            std::istringstream line_stream(line);

            char type = '\0';
            line_stream >> type;

            if (type == 'v') {
                std::array<double, 3> vertex;

                for (auto& coordinate : vertex) {
                    if (line_stream.eof()) {
                        throw std::runtime_error("Line " + std::to_string(line_number) + ": Vertex must have 3 coordinates");
                    }
                    line_stream >> coordinate;
                    if (coordinate + '0' == '#') {
                        throw std::runtime_error("Line " + std::to_string(line_number) + ": Vertex must have 3 coordinates");
                    }
                }

                vertices.push_back(vertex);
            }

            else if (type == 'f') {
                Triangle triangle;

                for (auto& vertex : triangle.vertices) {
                    if (line_stream.eof()) {
                        throw std::runtime_error("Line " + std::to_string(line_number) + ": Face must have 3 indices");
                    }
                    size_t index;
                    line_stream >> index;
                    if (index + '0' == '#') {
                        throw std::runtime_error("Line " + std::to_string(line_number) + ": Face must have 3 indices");
                    }
                    vertex = vertices[index - 1];
                }

                triangles.push_back(triangle);
            }

            else if (type != '#' and type != '\0' and type != 's') {
                throw std::runtime_error("Line " + std::to_string(line_number) + ": Unrecognised type '" + std::to_string(type) + "'");
            }

            line_number++;
        }
    }

    void transform(const Matrix4x4& matrix) {
        for (auto& triangle : triangles) {
            triangle.transform(matrix);
        }
    }
};
