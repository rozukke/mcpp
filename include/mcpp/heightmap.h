#pragma once

#include "coordinate.h"
#include <cstdint>
#include <memory>
#include <vector>

namespace mcpp {
/**
 * Represents a 2D area of the world with the y coordinates of the highest
 * non-air blocks at each (x,z)
 */
struct HeightMap {
private:
  Coordinate _base_pt;
  uint16_t _x_len;
  uint16_t _z_len;
  std::unique_ptr<int16_t[]> _raw_heights;

public:
  // Constructors and assignment
  HeightMap(const Coordinate& loc1, const Coordinate& loc2, const std::vector<int16_t>& heights);
  ~HeightMap() = default;

  HeightMap(const HeightMap& other)
      : _base_pt(other._base_pt), _x_len(other._x_len), _z_len(other._z_len) {
    size_t size = _x_len * _z_len;
    // Allocate memory and copy the heights
    _raw_heights.reset(new int16_t[size]);
    std::copy(other._raw_heights.get(), other._raw_heights.get() + size, _raw_heights.get());
  }

  HeightMap(HeightMap&& other) noexcept = default;
  HeightMap& operator=(const HeightMap& other);
  HeightMap& operator=(HeightMap&& other) = default;

  /**
   * Get the height using an offset from the origin/base point of the heights
   * area
   * @param x: x offset to access underlying array
   * @param z: z offset to access underlying array
   * @return: height at specified offset
   */
  int16_t get(int x, int z) const;

  /**
   * Get the height at a Minecraft coordinate if saved inside the height map
   * @param loc: Coordinate in Minecraft world to access in the map
   * @return: height at specified coordinate
   */
  int16_t get_worldspace(const Coordinate& loc) const;

  /**
   * Fill a coordinate inplace with the highest y coordinate at the `loc`'s x
   * and z components.
   * @param loc: Coordinate to fill y value for
   */
  void fill_coord(Coordinate& out) const;

  /**
   * Gets the x length of the HeightMap.
   * @return x length of the HeightMap
   */
  uint16_t x_len() const;

  /**
   * Gets the z length of the HeightMap.
   * @return z length of the HeightMap
   */
  uint16_t z_len() const;

  /**
   * Gets the minimum coordinate in the HeightMap.
   * @return the minimum coordinate in the HeightMap.
   */
  Coordinate base_pt() const;

  /**
   * @brief An iterator for the HeightMap structure.
   *
   * This iterator allows for range-based for loops and standard iterator
   * operations over the height data stored within a HeightMap.
   */
  struct Iterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = int16_t;
    using pointer = int16_t*;
    using reference = int16_t&;

    /**
     * @brief Constructs an iterator at the given pointer position.
     *
     * @param ptr Pointer to the position in the height array.
     */
    Iterator(pointer ptr) : m_ptr(ptr) {}

    /**
     * @brief Dereference the iterator to access the value at the current
     * position.
     *
     * @return Reference to the current element.
     */
    reference operator*() const { return *m_ptr; }

    /**
     * @brief Access the pointer to the current element.
     *
     * @return Pointer to the current element.
     */
    pointer operator->() { return m_ptr; }

    /**
     * @brief Pre-increment operator. Advances the iterator to the next
     * position.
     *
     * @return Reference to the updated iterator.
     */
    Iterator& operator++() {
      m_ptr++;
      return *this;
    }

    /**
     * @brief Post-increment operator. Advances the iterator to the next
     * position.
     *
     * @param int Unused dummy parameter to differentiate from prefix
     * increment.
     * @return Iterator to the original position before incrementing.
     */
    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    /**
     * @brief Equality comparison operator.
     *
     * @param a First iterator to compare.
     * @param b Second iterator to compare.
     * @return true if both iterators point to the same position, false
     * otherwise.
     */
    friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };

    /**
     * @brief Inequality comparison operator.
     *
     * @param a First iterator to compare.
     * @param b Second iterator to compare.
     * @return true if iterators point to different positions, false
     * otherwise.
     */
    friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

  private:
    pointer m_ptr;
  };

  /**
   * @brief An iterator for the const HeightMap structure.
   *
   * This iterator allows for range-based for loops and standard const
   * iterator operations over the height data stored within a HeightMap.
   */
  struct ConstIterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = int16_t;
    using pointer = const int16_t*;
    using reference = const int16_t&;

    /**
     * @brief Constructs an iterator at the given pointer position.
     *
     * @param ptr Pointer to the position in the height array.
     */
    ConstIterator(pointer ptr) : m_ptr(ptr) {}

    /**
     * @brief Dereference the iterator to access the value at the current
     * position.
     *
     * @return Reference to the current element.
     */
    reference operator*() const { return *m_ptr; }

    /**
     * @brief Access the pointer to the current element.
     *
     * @return Pointer to the current element.
     */
    pointer operator->() { return m_ptr; }

    /**
     * @brief Pre-increment operator. Advances the iterator to the next
     * position.
     *
     * @return Reference to the updated iterator.
     */
    ConstIterator& operator++() {
      m_ptr++;
      return *this;
    }

    /**
     * @brief Post-increment operator. Advances the iterator to the next
     * position.
     *
     * @param int Unused dummy parameter to differentiate from prefix
     * increment.
     * @return Iterator to the original position before incrementing.
     */
    ConstIterator operator++(int) {
      ConstIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    /**
     * @brief Equality comparison operator.
     *
     * @param a First iterator to compare.
     * @param b Second iterator to compare.
     * @return true if both iterators point to the same position, false
     * otherwise.
     */
    friend bool operator==(const ConstIterator& a, const ConstIterator& b) {
      return a.m_ptr == b.m_ptr;
    };

    /**
     * @brief Inequality comparison operator.
     *
     * @param a First iterator to compare.
     * @param b Second iterator to compare.
     * @return true if iterators point to different positions, false
     * otherwise.
     */
    friend bool operator!=(const ConstIterator& a, const ConstIterator& b) {
      return a.m_ptr != b.m_ptr;
    };

  private:
    pointer m_ptr;
  };

  Iterator begin() { return Iterator(&_raw_heights[0]); }
  Iterator end() { return Iterator(&_raw_heights[_x_len * _z_len]); }
  ConstIterator begin() const { return ConstIterator(&_raw_heights[0]); }
  ConstIterator end() const { return ConstIterator(&_raw_heights[_x_len * _z_len]); }
};

} // namespace mcpp
