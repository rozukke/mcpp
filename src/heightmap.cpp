#include "../include/mcpp/heightmap.h"
#include <cstdint>

namespace mcpp {
HeightMap::HeightMap(const Coordinate& loc1, const Coordinate& loc2,
                     const std::vector<int16_t>& heights) {
  _base_pt = Coordinate{
      std::min(loc1.x, loc2.x),
      0,
      std::min(loc1.z, loc2.z),
  };

  _x_len = std::abs(loc1.x - loc2.x) + 1;
  _z_len = std::abs(loc1.z - loc2.z) + 1;

  _raw_heights = std::make_unique<int16_t[]>(heights.size());
  std::copy(heights.begin(), heights.end(), _raw_heights.get());
}

HeightMap& HeightMap::operator=(const HeightMap& other) {
  if (this != &other) {
    // Copy data from the other object
    _base_pt = other._base_pt;
    _x_len = other._x_len;
    _z_len = other._z_len;
  }
  return *this;
}

int16_t HeightMap::get(int x, int z) const {
  if ((x < 0 || x >= _x_len) || (z < 0 || z >= _z_len)) {
    throw std::out_of_range("Out of range access of heightmap at " + std::to_string(x) + "," +
                            std::to_string(z) + " (worldspace x=" + std::to_string(_base_pt.x + x) +
                            ",z=" + std::to_string(_base_pt.z + z));
  }
  // Get 2D from flat vector
  return _raw_heights[(x * _z_len) + z];
}

int16_t HeightMap::get_worldspace(const Coordinate& loc) const {
  return get(loc.x - _base_pt.x, loc.z - _base_pt.z);
}

void HeightMap::fill_coord(Coordinate& out) const { out.y = get_worldspace(out); }

uint16_t HeightMap::x_len() const { return _x_len; }

uint16_t HeightMap::z_len() const { return _z_len; }

Coordinate HeightMap::base_pt() const { return _base_pt; }
} // namespace mcpp
