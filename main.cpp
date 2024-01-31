#include <iostream>

#include "Engine3D.hpp"

int main(int, char**) {
    try {
        Engine3D engine(600, 480);
        engine.run();
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}