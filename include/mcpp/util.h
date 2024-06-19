#pragma once

#include "block.h"
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
 * Stores a 3D cuboid of BlockTypes while preserving their relative location to
 * the base point they were gathered at and each other.
 */
struct Chunk {
    /**
     * Initialized by copying from a flat vector of blocks
     */
    Chunk(const Coordinate& loc1, const Coordinate& loc2,
          const std::vector<BlockType>& block_list);

    ~Chunk();

    /**
     * Accesses the Minecraft block at absolute position pos and returns its
     * BlockType if it is in the included area.
     * @param pos: Abolute position in the Minecraft world to query BlockType
     * for
     * @return BlockType at specified location
     */
    BlockType get_worldspace(const Coordinate& pos);

    /**
     * Local equivalent of get_worldspace, equivalent to a 3D array access of
     * the internal data.
     * @param x: x element of array access
     * @param y: y element of array access
     * @param z: z element of array access
     * @return BlockType at specified location
     */
    BlockType get(int x, int y, int z);

  private:
    Coordinate base_pt;
    BlockType* raw_data;
    int y_len;
    int x_len;
    int z_len;
};

} // namespace mcpp
