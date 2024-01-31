#pragma once

#include "Coordinate.hpp"

#include <SDL.h>

#include <string>
#include <array>
#include <vector>
#include <stdexcept>
#include <chrono>


class Renderer {
public:
    Renderer(int width = 640, int height = 480, const std::string& title = "Window");
    ~Renderer();
    void run();
    virtual void initialise();
    virtual void update(double frame_time);
    virtual void close();
protected:
    struct Pixel {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t alpha = 255;
    };
    static constexpr Pixel white = { 255, 255, 255 };
    void clear(const Pixel & = { 0, 0, 0 });
    void draw_pixel(const Coordinate&, const Pixel & = white);
    void draw_line(const Coordinate&, const Coordinate&, const Pixel & = white);
    void draw_triangle(std::array<Coordinate, 3>, const Pixel & = white);
    void draw_filled_triangle(std::array<Coordinate, 3>, const Pixel & = white);
    void draw_rectangle(const Coordinate&, const Coordinate&, const Pixel & = white);
    void draw_circle(const Coordinate&, int radius, const Pixel & = white);
    void sleep(int milliseconds);
    int width() const { return _width; }
    int height() const { return _height; }
    bool _running = true;
    enum class ButtonState {
        Pressed,
        Held,
        Released
    };
    enum class MouseButton {
        Left,
        Middle,
        Right
    };
    Coordinate mouse_position() const;
    ButtonState mouse_button(MouseButton) const;
    enum class Key : SDL_Keycode {
        Left = SDLK_LEFT,
        Right = SDLK_RIGHT,
        Up = SDLK_UP,
        Down = SDLK_DOWN,
        Space = SDLK_SPACE,
        Escape = SDLK_ESCAPE,
        Control = SDLK_LCTRL,
    };
    ButtonState key(Key) const;
    ButtonState key(char) const;
private:
    void handle_events();
    void render();
    const int _width;
    const int _height;
    SDL_Window* _window = nullptr;
    SDL_Renderer* _renderer = nullptr;
    SDL_Texture* _screen = nullptr;
    std::vector<std::vector<Pixel>> _pixels;
    double _time_elapsed = 0;
    Coordinate _mouse_position;
    std::array<ButtonState, 5> _mouse_buttons;
    std::array<ButtonState, 128> _keys;
};


class SDLException final : public std::runtime_error {
public:
    explicit SDLException(const std::string& message) : std::runtime_error(message + "(SDL_Error: " + std::string(SDL_GetError()) + ")\n") {}
};


namespace Timer {
    void start();
    void stop();
    double elapsed();
    void restart();
    static struct {
        std::chrono::high_resolution_clock::time_point start;
        std::chrono::high_resolution_clock::time_point stop;
    } time;
};