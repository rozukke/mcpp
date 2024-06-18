#pragma once

#include <ostream>
#include <vector>

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

/**
 * Represents a 2D area of the world with the y coordinates of the highest
 * non-air blocks at each (x,z)
 */
struct HeightMap {
    HeightMap(const Coordinate& loc1, const Coordinate& loc2,
              const std::vector<int>& heights);

    /**
     * Get the height using an offset from the origin/base point of the heights
     * area
     * @param x: x offset to access underlying array
     * @param z: z offset to access underlying array
     * @return: height at specified offset
     */
    int get(int x, int z) const;

    /**
     * Get the height at a Minecraft coordinate if saved inside the height map
     * @param loc: Coordinate in Minecraft world to access in the map
     * @return: height at specified coordinate
     */
    int get_worldspace(const Coordinate& loc) const;

    /**
     * Fill a coordinate inplace with the highest y coordinate at the `loc`'s x
     * and z components.
     * @param loc: Coordinate to fill y value for
     */
    void fill_coord(Coordinate& out);

  private:
    int x_len;
    int z_len;
    Coordinate base_pt;
    int* raw_heights;
};

} // namespace mcpp
