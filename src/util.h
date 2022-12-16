#pragma once

class Coordinate {
    // is represented using ints since sub-grid coordinates are not particularly relevant
public:
    explicit Coordinate(int x = 0, int y = 0, int z = 0);
    Coordinate(double x, double y, double z);
    Coordinate operator + (const Coordinate &obj) const;
    bool operator == (const Coordinate &obj) const;
    Coordinate operator - (const Coordinate &obj) const;
    [[nodiscard]] Coordinate clone() const;

    int x;
    int y;
    int z;
};