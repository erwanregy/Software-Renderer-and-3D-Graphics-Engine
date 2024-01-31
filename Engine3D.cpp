#include "Engine3D.hpp"

#include <algorithm>


void Engine3D::update(const double frame_time) {
    clear();

    if (key('1') == ButtonState::Pressed) {
        _drawing_mode = DrawingMode::Filled;
    } else if (key('2') == ButtonState::Pressed) {
        _drawing_mode = DrawingMode::WireFrame;
    } else if (key('3') == ButtonState::Pressed) {
        _drawing_mode = DrawingMode::Both;
    }

    if (key(Key::Space) == ButtonState::Held) {
        _camera.position.y += 5 * frame_time;
    } else if (key('z') == ButtonState::Held) {
        _camera.position.y -= 5 * frame_time;
    }

    if (key('w') == ButtonState::Held) {
        _camera.position += _camera.direction * 5 * frame_time;
    }
    if (key('s') == ButtonState::Held) {
        _camera.position -= _camera.direction * 5 * frame_time;
    }

    const auto right = cross(_camera.direction, { 0, -1, 0 }).normalised();
    if (key('a') == ButtonState::Held) {
        _camera.position -= right * 5 * frame_time;
    }
    if (key('d') == ButtonState::Held) {
        _camera.position += right * 5 * frame_time;
    }

    /*if (GetKey(olc::Key::LEFT).bHeld) {
        _camera.yaw -= 2 * frame_time;
    }
    if (GetKey(olc::Key::RIGHT).bHeld) {
        _camera.yaw += 2 * frame_time;
    }

    if (GetKey(olc::Key::UP).bHeld) {
        _camera.pitch -= 2 * frame_time;
    }
    if (GetKey(olc::Key::DOWN).bHeld) {
        _camera.pitch += 2 * frame_time;
    }*/

    constexpr double pi = 3.14159265358979323846;

    _camera.yaw = 2 * pi * mouse_position().x / static_cast<double>(width()) - pi;
    _camera.pitch = 2 * pi * mouse_position().y / static_cast<double>(height()) - pi;

    _camera.pitch = std::clamp(_camera.pitch, -pi / 2, pi / 2);

    if (key('r') == ButtonState::Pressed) {
        _rotation = { 0, 0, 0 };
    }
    if (key('t') == ButtonState::Pressed) {
        _auto_rotate = not _auto_rotate;
    }
    if (_auto_rotate) {
        _rotation += {frame_time, 0, frame_time / 2};
    }

    const auto world_matrix = make_translation_matrix(0, 0, 15) * make_rotation_matrix(_rotation);

    const auto camera_rotation_matrix = make_rotation_matrix_y(_camera.yaw) * make_rotation_matrix_x(_camera.pitch);
    const auto target = Vector3D(0, 0, 1);
    _camera.direction = camera_rotation_matrix * target;
    const auto view_matrix = make_view_matrix(_camera.position, _camera.position + _camera.direction);

    Mesh visible_mesh;

    for (const auto& mesh : _meshes) {
        for (auto triangle : mesh.triangles) {
            // Rotate and translate
            for (auto& vertex : triangle.vertices) {
                vertex *= world_matrix;
            }

            if (_drawing_mode == DrawingMode::Filled) {
                // Back face culling
                if (auto camera_ray = triangle.vertices[0] - _camera.position;
                    dot(triangle.normal(), camera_ray.normalised()) > 0) {
                    continue;
                }
            }

            const auto light_direction = Vector3D(0, 0, -1).normalised();
            triangle.illumination = dot(triangle.normal(), light_direction);

            for (auto& vertex : triangle.vertices) {
                // View
                vertex *= view_matrix;

                // Project
                vertex *= _projection_matrix;
                if (vertex.w != 0) {
                    vertex /= vertex.w;
                }

                // Scale into view
                vertex += { 1, 1, 0 };
                vertex.x *= static_cast<double>(width()) / 2;
                vertex.y *= static_cast<double>(height()) / 2;
            }

            visible_mesh.triangles.push_back(triangle);
        }
    }

    std::ranges::sort(
        visible_mesh.triangles.begin(), visible_mesh.triangles.end(),
        [](const Triangle& a, const Triangle& b) { return a.depth() < b.depth(); }
    );

    draw_mesh(visible_mesh);
    //draw_wire_frame_mesh(visible_mesh);
}

void Engine3D::draw_mesh(const Mesh& mesh) {
    switch (_drawing_mode) {
        case DrawingMode::Filled:
            draw_filled_mesh(mesh);
            break;
        case DrawingMode::WireFrame:
            draw_wire_frame_mesh(mesh);
            break;
        case DrawingMode::Both:
            draw_filled_mesh(mesh);
            draw_wire_frame_mesh(mesh);
            break;
        default:
            throw std::runtime_error("Invalid drawing mode");
    }
}

void Engine3D::draw_filled_mesh(const Mesh& mesh) {
    for (const auto& triangle : mesh.triangles) {
        std::array<Coordinate, 3> coordinates;
        for (uint8_t i = 0; i < 3; ++i) {
            coordinates[i] = { static_cast<int>(triangle.vertices[i].x), static_cast<int>(triangle.vertices[i].y) };
        }
        const auto brightness = static_cast<uint8_t>(triangle.illumination * 255);
        draw_filled_triangle(coordinates, { brightness, brightness, brightness });
    }
}

void Engine3D::draw_wire_frame_mesh(const Mesh& mesh) {
    for (const auto& triangle : mesh.triangles) {
        std::array<Coordinate, 3> coordinates;
        for (uint8_t i = 0; i < 3; ++i) {
            coordinates[i] = { static_cast<int>(triangle.vertices[i].x), static_cast<int>(triangle.vertices[i].y) };
        }
        //const auto brightness = static_cast<uint8_t>(triangle.illumination * 255);
        //draw_triangle(coordinates, { brightness, brightness, brightness });
        draw_triangle(coordinates, { 0, 255, 0 });
    }
}