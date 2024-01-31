#pragma once

#include <iostream>


struct Coordinate {

    int x, y;

    Coordinate& operator-() { x = -x; y = -y; return *this; }

    Coordinate& operator+=(const Coordinate& other) { return *this = *this + other; }
    Coordinate& operator-=(const Coordinate& other) { return *this = *this - other; }
    Coordinate& operator*=(const Coordinate& other) { return *this = *this * other; }
    Coordinate& operator/=(const Coordinate& other) { return *this = *this / other; }

    Coordinate& operator*=(const int scalar) { return *this = *this * scalar; }
    Coordinate& operator/=(const int scalar) { return *this = *this / scalar; }

    bool in_bounds(const Coordinate& dimensions) const { return *this >= Coordinate{ 0, 0 } and *this < dimensions; }

    friend Coordinate operator+(const Coordinate& lhs, const Coordinate& rhs) { return { lhs.x + rhs.x, lhs.y + rhs.y }; }
    friend Coordinate operator-(const Coordinate& lhs, const Coordinate& rhs) { return { lhs.x - rhs.x, lhs.y - rhs.y }; }
    friend Coordinate operator*(const Coordinate& lhs, const Coordinate& rhs) { return { lhs.x * rhs.x, lhs.y * rhs.y }; }
    friend Coordinate operator/(const Coordinate& lhs, const Coordinate& rhs) { return { lhs.x / rhs.x, lhs.y / rhs.y }; }

    friend Coordinate operator*(const Coordinate& lhs, const int rhs) { return { lhs.x * rhs, lhs.y * rhs }; }
    friend Coordinate operator*(const int lhs, const Coordinate& rhs) { return rhs * lhs; }
    friend Coordinate operator/(const Coordinate& lhs, const int rhs) { return { lhs.x / rhs, lhs.y / rhs }; }

    friend bool operator< (const Coordinate& lhs, const Coordinate& rhs) { return lhs.x < rhs.x and lhs.y < rhs.y; }
    friend bool operator<=(const Coordinate& lhs, const Coordinate& rhs) { return lhs.x <= rhs.x and lhs.y <= rhs.y; }
    friend bool operator==(const Coordinate& lhs, const Coordinate& rhs) { return lhs.x == rhs.x and lhs.y == rhs.y; }
    friend bool operator>=(const Coordinate& lhs, const Coordinate& rhs) { return lhs.x >= rhs.x and lhs.y >= rhs.y; }
    friend bool operator> (const Coordinate& lhs, const Coordinate& rhs) { return lhs.x > rhs.x and lhs.y > rhs.y; }

    friend std::wostream& operator<<(std::wostream& stream, const Coordinate& coordinate) { return stream << coordinate.x << L' ' << coordinate.y; }
    friend std::wistream& operator>>(std::wistream& stream, Coordinate& coordinate) { return stream >> coordinate.x >> coordinate.y; }
};