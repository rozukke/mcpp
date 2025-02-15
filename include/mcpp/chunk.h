#pragma once

#include "block.h"
#include "coordinate.h"
#include <memory>
#include <vector>

namespace mcpp {
/**
 * Stores a 3D cuboid of BlockTypes while preserving their relative location to
 * the base point they were gathered at and each other.
 */
struct Chunk {
private:
  Coordinate _base_pt;
  uint16_t _x_len;
  uint16_t _y_len;
  uint16_t _z_len;
  std::unique_ptr<BlockType[]> _raw_data;

public:
  // Constructors and assignment
  Chunk(const Coordinate& loc1, const Coordinate& loc2, const std::vector<BlockType>& block_list);
  ~Chunk() = default;

  Chunk(const Chunk& other)
      : _base_pt(other._base_pt), _x_len(other._x_len), _y_len(other._y_len), _z_len(other._z_len) {
    size_t size = _x_len * _y_len * _z_len;
    _raw_data.reset(new BlockType[size]);
    std::copy(other._raw_data.get(), other._raw_data.get() + size, _raw_data.get());
  }

  Chunk(Chunk&& other) noexcept = default;
  Chunk& operator=(const Chunk& other);
  Chunk& operator=(Chunk&& other) = default;

  /**
   * Accesses the Minecraft block at absolute position pos and returns its
   * BlockType if it is in the included area.
   * @param pos: Abolute position in the Minecraft world to query BlockType
   * for
   * @return BlockType at specified location
   */
  BlockType get_worldspace(const Coordinate& pos) const;

  /**
   * Local equivalent of get_worldspace, equivalent to a 3D array access of
   * the internal data.
   * @param x: x element of array access
   * @param y: y element of array access
   * @param z: z element of array access
   * @return BlockType at specified location
   */
  BlockType get(int x, int y, int z) const;

  /**
   * Gets the x length of the Chunk.
   * @return x length of the Chunk
   */
  uint16_t x_len() const;

  /**
   * Gets the y length of the Chunk.
   * @return y length of the Chunk
   */
  uint16_t y_len() const;

  /**
   * Gets the z length of the Chunk.
   * @return z length of the Chunk
   */
  uint16_t z_len() const;

  /**
   * Gets the minimum coordinate in the Chunk.
   * @return the minimum coordinate in the Chunk
   */
  Coordinate base_pt() const;

  /**
   * @brief An iterator for the Chunk's 3D block data.
   *
   * This iterator allows for range-based for loops and standard iterator
   * operations over the 3D block data stored within a Chunk. It provides a
   * linear interface to traverse the 3D grid of blocks, enabling sequential
   * access to the elements stored in the chunk.
   */
  struct Iterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = BlockType;
    using pointer = BlockType*;
    using reference = BlockType&;

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
   * @brief An iterator for the const Chunk's 3D block data.
   *
   * This iterator allows for range-based for loops and standard const
   * iterator operations over the 3D block data stored within a Chunk. It
   * provides a linear interface to traverse the 3D grid of blocks, enabling
   * sequential immutable access to the elements stored in the chunk.
   */
  struct ConstIterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = BlockType;
    using pointer = const BlockType*;
    using reference = const BlockType&;

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

  // Iterators
  Iterator begin() { return Iterator(&_raw_data[0]); }
  Iterator end() { return Iterator(&_raw_data[_x_len * _y_len * _z_len]); }
  ConstIterator begin() const { return ConstIterator(&_raw_data[0]); }
  ConstIterator end() const { return ConstIterator(&_raw_data[_x_len * _y_len * _z_len]); }
};
} // namespace mcpp
