#include <memory>

#include "../include/mcpp/chunk.h"

namespace mcpp {
Chunk::Chunk(const Coordinate& loc1, const Coordinate& loc2,
             const std::vector<BlockType>& block_list) {
  Coordinate min{std::min(loc1.x, loc2.x), std::min(loc1.y, loc2.y), std::min(loc1.z, loc2.z)};
  _base_pt = min;

  Coordinate dim = loc1 - loc2;
  _x_len = std::abs(dim.x) + 1;
  _y_len = std::abs(dim.y) + 1;
  _z_len = std::abs(dim.z) + 1;

  _raw_data = std::make_unique<BlockType[]>(block_list.size());
  std::copy(block_list.begin(), block_list.end(), _raw_data.get());
}

Chunk& Chunk::operator=(const Chunk& other) {
  if (this != &other) {
    _base_pt = other._base_pt;
    _x_len = other._x_len;
    _y_len = other._y_len;
    _z_len = other._z_len;
    size_t size = _x_len * _y_len * _z_len;
    _raw_data = std::make_unique<BlockType[]>(size);
    std::copy(other._raw_data.get(), other._raw_data.get() + size, _raw_data.get());
  }
  return *this;
}

BlockType Chunk::get(int x, int y, int z) const {
  if ((x < 0 || y < 0 || z < 0) || (x > _x_len - 1 || y > _y_len - 1 || z > _z_len - 1)) {
    throw std::out_of_range("Out of bounds Chunk access at " + to_string(Coordinate(x, y, z)));
  }
  return _raw_data[(y * _x_len * _z_len) + (x * _z_len) + z];
}

BlockType Chunk::get_worldspace(const Coordinate& pos) const {
  Coordinate array_pos = pos - _base_pt;
  if ((array_pos.x < 0 || array_pos.y < 0 || array_pos.z < 0) ||
      (array_pos.x > _x_len - 1 || array_pos.y > _y_len - 1 || array_pos.z > _z_len - 1)) {
    throw std::out_of_range("Out of bounds Chunk access at " + to_string(array_pos) +
                            " (world coordinate " + to_string(pos) + " )");
  }
  return _raw_data[(array_pos.y * _x_len * _z_len) + (array_pos.x * _z_len) + array_pos.z];
}

uint16_t Chunk::x_len() const { return this->_x_len; }

uint16_t Chunk::y_len() const { return this->_y_len; }

uint16_t Chunk::z_len() const { return this->_z_len; }

Coordinate Chunk::base_pt() const { return this->_base_pt; }
} // namespace mcpp
