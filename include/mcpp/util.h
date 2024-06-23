#pragma once

#include <ostream>
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
} // namespace mcpp
