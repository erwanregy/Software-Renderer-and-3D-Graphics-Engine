#pragma once

#include "Renderer.hpp"

#include "Mesh.hpp"
#include "Matrix4x4.hpp"
#include "Vector3D.hpp"


class Engine3D final : public Renderer {
public:
    using Renderer::Renderer;
    void update(double frame_time) override;
private:
    std::vector<Mesh> _meshes = {
        Mesh("meshes/teapot.obj"),
        //Mesh("meshes/axes.obj"),
    };

    double _field_of_view = 90;
    double _near_plane = 0.1;
    double _far_plane = 1000;

    Matrix4x4 _projection_matrix = make_projection_matrix({ width(), height() }, _field_of_view, _near_plane, _far_plane);

    bool _auto_rotate = false;
    Vector3D _rotation = { 0, 0, 0 };

    struct {
        Vector3D position = { 0, 2, 10 };
        Vector3D direction = { 0, 0, 1 };
        double yaw = 0;
        double pitch = 0;
    } _camera;

    enum class DrawingMode : uint8_t {
        WireFrame,
        Filled,
        Both
    } _drawing_mode = DrawingMode::WireFrame;

    void draw_mesh(const Mesh& mesh);
    void draw_filled_mesh(const Mesh&);
    void draw_wire_frame_mesh(const Mesh&);
};