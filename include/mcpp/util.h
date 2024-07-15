#pragma once

#include <ostream>
#include <vector>

/** @file
 * @brief Coordinate class.
 *
 */
namespace mcpp {
/**
 * Represented using integers since sub-unit coordinates are not of particular
 * relevance. Allows for operations such as addition between coordinates.
 */
class Coordinate {
  public:
    /**
     * @brief Constructs a Coordinate object with integer values.
     *
     * @param x The x-coordinate. Default is 0.
     * @param y The y-coordinate. Default is 0.
     * @param z The z-coordinate. Default is 0.
     */
    explicit Coordinate(int x = 0, int y = 0, int z = 0);

    /**
     * @brief Constructs a Coordinate object with double values.
     *
     * @param x The x-coordinate as a double.
     * @param y The y-coordinate as a double.
     * @param z The z-coordinate as a double.
     */
    Coordinate(double x, double y, double z);

    /**
     * @brief Adds two Coordinate objects.
     *
     * @param obj The Coordinate object to add.
     * @return A new Coordinate object representing the sum of the two
     * coordinates.
     */
    Coordinate operator+(const Coordinate& obj) const;

    /**
     * @brief Checks if two Coordinate objects are equal.
     *
     * @param obj The Coordinate object to compare with.
     * @return True if the coordinates are equal, false otherwise.
     */
    bool operator==(const Coordinate& obj) const;

    /**
     * @brief Checks if two Coordinate objects are not equal.
     *
     * @param obj The Coordinate object to compare with.
     * @return True if the coordinates are not equal, false otherwise.
     */
    bool operator!=(const Coordinate& obj) const;

    /**
     * @brief Subtracts one Coordinate object from another.
     *
     * @param obj The Coordinate object to subtract.
     * @return A new Coordinate object representing the difference between the
     * two coordinates.
     */
    Coordinate operator-(const Coordinate& obj) const;

    /**
     * @brief Creates a copy of the Coordinate object.
     *
     * @return A new Coordinate object that is a copy of the current object.
     */
    [[nodiscard]] Coordinate clone() const;

    /**
     * @brief Outputs the Coordinate object to an ostream.
     *
     * @param out The output stream.
     * @param coord The Coordinate object to output.
     * @return The output stream with the Coordinate object's values.
     */
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
