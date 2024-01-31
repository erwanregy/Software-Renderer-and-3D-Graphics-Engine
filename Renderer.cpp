#include "Renderer.hpp"

#include <algorithm>


Renderer::Renderer(const int width, const int height, const std::string& title) : _width(width), _height(height), _pixels(width, std::vector<Pixel>(height)) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw SDLException("SDL could not initialize");
    }

    _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (_window == nullptr) {
        throw SDLException("Window could not be created");
    }

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (_renderer == nullptr) {
        throw SDLException("Renderer could not be created");
    }

    _screen = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, _width, _height);
    if (_screen == nullptr) {
        throw SDLException("Screen texture could not be created");
    }
}

Renderer::~Renderer() {
    SDL_DestroyWindow(_window);
    _window = nullptr;
    SDL_Quit();
}

void Renderer::run() {
    Timer::start();
    sleep(1);

    initialise();

    while (_running) {
        Timer::stop();

        const double frame_time = Timer::elapsed();
        const double frame_rate = 1 / frame_time;
        std::cout << "Frame rate: " << frame_rate << '\r';

        Timer::restart();

        handle_events();

        update(frame_time);

        render();
    }

    close();
}

Coordinate Renderer::mouse_position() const {
    return _mouse_position;
}

Renderer::ButtonState Renderer::mouse_button(MouseButton b) const {
    const auto button = static_cast<uint8_t>(b);
    if (button >= _mouse_buttons.size() or button < 0) {
        throw std::runtime_error("Invalid mouse button " + std::to_string(button));
    }
    return _mouse_buttons[button];
}

Renderer::ButtonState Renderer::key(const Key k) const {
    return key(static_cast<char>(k));
}

Renderer::ButtonState Renderer::key(const char key) const {
    if (key >= _keys.size() or key < 0) {
        throw std::runtime_error("Invalid key " + std::to_string(key));
    }
    return _keys[key];
}

void Renderer::handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_QUIT: {
                _running = false;
                break;
            }
            case SDL_KEYDOWN: {
                if (_keys[event.key.keysym.sym] == ButtonState::Pressed) {
                    _keys[event.key.keysym.sym] = ButtonState::Held;
                } else {
                    _keys[event.key.keysym.sym] = ButtonState::Pressed;
                }
                break;
            }
            case SDL_KEYUP: {
                _keys[event.key.keysym.sym] = ButtonState::Released;
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                if (_mouse_buttons[event.button.button] == ButtonState::Pressed) {
                    _mouse_buttons[event.button.button] = ButtonState::Held;
                } else {
                    _mouse_buttons[event.button.button] = ButtonState::Pressed;
                }
                break;
            }
            case SDL_MOUSEBUTTONUP: {
                _mouse_buttons[event.button.button] = ButtonState::Released;
                break;
            }
            case SDL_MOUSEMOTION: {
                _mouse_position.x = event.motion.x;
                _mouse_position.y = event.motion.y;
                break;
            }
            default: {
                break;
            }
        }
    }
}

void Renderer::initialise() {}

void Renderer::update(const double frame_time) {
    clear();
    draw_filled_triangle({ { { rand() % _width, rand() % _height }, { rand() % _width, rand() % _height }, { rand() % _width, rand() % _height } } });
    sleep(100);
}

void Renderer::close() {}

void Renderer::clear(const Pixel& pixel) {
    for (int x = 0; x < _width; ++x) {
        for (int y = 0; y < _height; ++y) {
            _pixels[x][y] = pixel;
        }
    }
}

void Renderer::draw_pixel(const Coordinate& coordinate, const Pixel& pixel) {
    if (coordinate.x >= _width or coordinate.x < 0 or coordinate.y >= _height or coordinate.y < 0) {
        return;
    }
    _pixels[coordinate.x][coordinate.y] = pixel;
}

void Renderer::draw_line(const Coordinate& start, const Coordinate& end, const Pixel& pixel) {
    Coordinate current = start;
    const Coordinate delta = {
        std::abs(end.x - start.x),
        std::abs(end.y - start.y)
    };
    const Coordinate step = {
        start.x < end.x ? 1 : -1,
        start.y < end.y ? 1 : -1
    };

    if (delta.x > delta.y) {
        int error = delta.x / 2;
        while (current.x != end.x) {
            draw_pixel(current, pixel);
            error -= delta.y;
            if (error < 0) {
                current.y += step.y;
                error += delta.x;
            }
            current.x += step.x;
        }
    } else {
        int error = delta.y / 2;
        while (current.y != end.y) {
            draw_pixel(current, pixel);
            error -= delta.x;
            if (error < 0) {
                current.x += step.x;
                error += delta.y;
            }
            current.y += step.y;
        }
    }
}

void Renderer::draw_triangle(std::array<Coordinate, 3> coordinates, const Pixel& pixel) {
    draw_line(coordinates[0], coordinates[1], pixel);
    draw_line(coordinates[1], coordinates[2], pixel);
    draw_line(coordinates[2], coordinates[0], pixel);
}

void Renderer::draw_filled_triangle(std::array<Coordinate, 3> coordinates, const Pixel& pixel) {
    std::ranges::sort(coordinates.begin(), coordinates.end(), [](const Coordinate& a, const Coordinate& b) { return a.y < b.y; });
    const auto [top, middle, bottom] = coordinates;

    const double slope_top_to_middle = static_cast<double>(middle.x - top.x) / (middle.y - top.y);
    const double slope_top_to_bottom = static_cast<double>(bottom.x - top.x) / (bottom.y - top.y);
    const double slope_middle_to_bottom = static_cast<double>(bottom.x - middle.x) / (bottom.y - middle.y);

    double x1 = top.x;
    double x2 = top.x + 0.5;
    for (int y = top.y; y <= middle.y; ++y) {
        draw_line({ static_cast<int>(x1), y }, { static_cast<int>(x2), y }, pixel);
        x1 += slope_top_to_middle;
        x2 += slope_top_to_bottom;
    }

    x1 = middle.x;
    x2 = middle.x + 0.5;
    for (int y = middle.y; y <= bottom.y; ++y) {
        draw_line({ static_cast<int>(x1), y }, { static_cast<int>(x2), y }, pixel);
        x1 += slope_middle_to_bottom;
        x2 += slope_top_to_bottom;
    }
}

void Renderer::draw_rectangle(const Coordinate& top_left, const Coordinate& bottom_right, const Pixel& pixel) {
    const Coordinate top_right = { bottom_right.x, top_left.y };
    const Coordinate bottom_left = { top_left.x, bottom_right.y };
    draw_line(top_left, top_right, pixel);
    draw_line(top_right, bottom_right, pixel);
    draw_line(bottom_right, bottom_left, pixel);
    draw_line(bottom_left, top_left, pixel);
}

void Renderer::sleep(const int milliseconds) {
    SDL_Delay(milliseconds);
}

void Renderer::render() {
    SDL_RenderClear(_renderer);
    const auto pixels = new uint32_t[_width * _height];
    for (int x = 0; x < _width; ++x) {
        for (int y = 0; y < _height; ++y) {
            pixels[x + y * _width] = _pixels[x][y].red << 24 | _pixels[x][y].green << 16 | _pixels[x][y].blue << 8 | _pixels[x][y].alpha;
        }
    }
    SDL_UpdateTexture(_screen, nullptr, pixels, _width * 4);
    delete[] pixels;
    SDL_RenderCopy(_renderer, _screen, nullptr, nullptr);
    SDL_RenderPresent(_renderer);
    SDL_RenderSetLogicalSize(_renderer, _width, _height);
    // SDL_RenderSetIntegerScale(_renderer, SDL_TRUE);
}


void Timer::start() { time.start = std::chrono::high_resolution_clock::now(); }
void Timer::stop() { time.stop = std::chrono::high_resolution_clock::now(); }
double Timer::elapsed() { return std::chrono::duration_cast<std::chrono::duration<double>>(time.stop - time.start).count(); }
void Timer::restart() { time.start = time.stop; }