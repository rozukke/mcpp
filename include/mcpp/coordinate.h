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
struct Coordinate {
  int32_t x;
  int32_t y;
  int32_t z;

  /**
   * @brief Constructs a Coordinate object with integer values.
   *
   * @param x The x-coordinate.
   * @param y The y-coordinate.
   * @param z The z-coordinate.
   */
  constexpr Coordinate(int32_t x, int32_t y, int32_t z) : x(x), y(y), z(z) {}

  /**
   * @brief Constructs a Coordinate object with zero values.
   */
  constexpr Coordinate() : x(0), y(0), z(0) {}

  /**
   * @brief Constructs a Coordinate object with double values.
   *
   * @param x The x-coordinate as a double.
   * @param y The y-coordinate as a double.
   * @param z The z-coordinate as a double.
   */
  constexpr Coordinate(double x, double y, double z)
      : x(static_cast<int>(x)), y(static_cast<int>(y)), z(static_cast<int>(z)) {}

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
   * @brief Implements hash algorithm for Coordinate object using non-negative
   * mapping and weighted coordinate values.
   *
   * @param obj The Coordinate object to hash.
   * @return Hash of Coordinate object.
   */
  std::size_t operator()(const Coordinate& obj) const;

  /**
   * @brief Outputs the Coordinate object to an ostream.
   *
   * @param out The output stream.
   * @param coord The Coordinate object to output.
   * @return The output stream with the Coordinate object's values.
   */
  friend std::ostream& operator<<(std::ostream& out, const Coordinate& coord);
};

/**
 * @brief Convert coordinate to string representation.
 *
 * @param coord The coordinate to stringify
 * @return stringified coordinate
 */
std::string to_string(const Coordinate& coord);

} // namespace mcpp
