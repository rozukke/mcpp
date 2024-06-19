#pragma once

#include <ostream>

namespace mcpp {
/**
 * Represented using integers since sub-unit coordinates are not of particular
 * relevance. Allows for operations such as addition between coordinates.
 */
class Coordinate {
  public:
    explicit Coordinate(int x = 0, int y = 0, int z = 0);

    Coordinate(double x, double y, double z);

    Coordinate operator+(const Coordinate& obj) const;

    bool operator==(const Coordinate& obj) const;

    bool operator!=(const Coordinate& obj) const;

    Coordinate operator-(const Coordinate& obj) const;

    [[nodiscard]] Coordinate clone() const;

    friend std::ostream& operator<<(std::ostream& out, const Coordinate& coord);

    int x;
    int y;
    int z;
};
} // namespace mcpp
