#include "../include/mcpp/util.h"
#include <stdexcept>
#include <string>
#include <vector>

namespace mcpp {

Coordinate::Coordinate(int x, int y, int z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Coordinate::Coordinate(double x, double y, double z) {
    this->x = static_cast<int>(x);
    this->y = static_cast<int>(y);
    this->z = static_cast<int>(z);
}

Coordinate Coordinate::operator+(const Coordinate& obj) const {
    Coordinate result;
    result.x = this->x + obj.x;
    result.y = this->y + obj.y;
    result.z = this->z + obj.z;
    return result;
}

bool Coordinate::operator==(const Coordinate& obj) const {
    return (this->x == obj.x) && (this->y == obj.y) && (this->z == obj.z);
}

bool Coordinate::operator!=(const Coordinate& obj) const {
    return !(*this == obj);
}

Coordinate Coordinate::operator-(const Coordinate& obj) const {
    Coordinate result;
    result.x = this->x - obj.x;
    result.y = this->y - obj.y;
    result.z = this->z - obj.z;
    return result;
}

std::size_t Coordinate::operator()(const mcpp::Coordinate& obj) const {
    int lower = -3e7, upper = 3e7;
    size_t base = upper - lower + 1;

    // Make x,y,z non negative
    size_t nx = obj.x - lower, ny = obj.y - lower, nz = obj.z - lower;

    // Use overflow instead of modding
    return nx * base * base + ny * base + nz;
}

Coordinate Coordinate::clone() const {
    return Coordinate(this->x, this->y, this->z);
}

std::string to_string(const Coordinate& coord) {
    using std::to_string;
    return "(" + to_string(coord.x) + "," + to_string(coord.y) + "," +
           to_string(coord.z) + ")";
}

std::ostream& operator<<(std::ostream& out, const Coordinate& coord) {
    out << to_string(coord);
    return out;
}

Chunk::Chunk(const Coordinate& loc1, const Coordinate& loc2,
             const std::vector<BlockType>& block_list) {
    Coordinate min{std::min(loc1.x, loc2.x), std::min(loc1.y, loc2.y),
                   std::min(loc1.z, loc2.z)};
    this->_base_pt = min.clone();

    Coordinate dim = loc1 - loc2;
    _x_len = std::abs(dim.x) + 1;
    _y_len = std::abs(dim.y) + 1;
    _z_len = std::abs(dim.z) + 1;

    this->raw_data = new BlockType[block_list.size()];
    std::copy(block_list.begin(), block_list.end(), raw_data);
}

Chunk::~Chunk() { delete[] raw_data; }

Chunk& Chunk::operator=(const Chunk& other) noexcept {
    if (this != &other) {
        // Clean up existing resource
        delete[] raw_data;

        // Copy data from the other object
        _base_pt = other._base_pt.clone();
        _x_len = other._x_len;
        _y_len = other._y_len;
        _z_len = other._z_len;
        raw_data = new BlockType[_x_len * _y_len * _z_len];
        std::copy(other.raw_data, other.raw_data + _x_len * _y_len * _z_len,
                  raw_data);
    }
    return *this;
}

BlockType Chunk::get(int x, int y, int z) const {
    if ((x < 0 || y < 0 || z < 0) ||
        (x > _x_len - 1 || y > _y_len - 1 || z > _z_len - 1)) {
        throw std::out_of_range("Out of bounds Chunk access at " +
                                to_string(Coordinate(x, y, z)));
    }
    return raw_data[y * _x_len * _z_len + x * _z_len + z];
}

BlockType Chunk::get_worldspace(const Coordinate& pos) const {
    Coordinate array_pos = pos - _base_pt;
    if ((array_pos.x < 0 || array_pos.y < 0 || array_pos.z < 0) ||
        (array_pos.x > _x_len - 1 || array_pos.y > _y_len - 1 ||
         array_pos.z > _z_len - 1)) {
        throw std::out_of_range("Out of bounds Chunk access at " +
                                to_string(array_pos) + " (world coordinate " +
                                to_string(pos) + " )");
    }
    return raw_data[array_pos.y * _x_len * _z_len + array_pos.x * _z_len +
                    array_pos.z];
}

int Chunk::x_len() const { return this->_x_len; }

int Chunk::y_len() const { return this->_y_len; }

int Chunk::z_len() const { return this->_z_len; }

Coordinate Chunk::base_pt() const { return this->_base_pt.clone(); }

HeightMap::HeightMap(const Coordinate& loc1, const Coordinate& loc2,
                     const std::vector<int>& heights) {
    _base_pt = Coordinate{
        std::min(loc1.x, loc2.x),
        0,
        std::min(loc1.z, loc2.z),
    };

    _x_len = std::abs(loc1.x - loc2.x) + 1;
    _z_len = std::abs(loc1.z - loc2.z) + 1;

    raw_heights = new int[heights.size()];
    std::copy(heights.begin(), heights.end(), raw_heights);
}

HeightMap::~HeightMap() { delete[] raw_heights; }

HeightMap& HeightMap::operator=(const HeightMap& other) noexcept {
    if (this != &other) {
        // Free the existing resource
        delete[] raw_heights;

        // Copy data from the other object
        _base_pt = other._base_pt.clone();
        _x_len = other._x_len;
        _z_len = other._z_len;

        // Allocate memory and copy the heights
        raw_heights = new int[_x_len * _z_len];
        std::copy(other.raw_heights, other.raw_heights + _x_len * _z_len,
                  raw_heights);
    }
    return *this;
}

int HeightMap::get(int x, int z) const {
    if ((x < 0 || x >= _x_len) || (z < 0 || z >= _z_len)) {
        throw new std::out_of_range(
            "Out of range access of heightmap at " + std::to_string(x) + "," +
            std::to_string(z) +
            " (worldspace x=" + std::to_string(_base_pt.x + x) +
            ",z=" + std::to_string(_base_pt.z + z));
    }
    // Get 2D from flat vector
    return raw_heights[x * _z_len + z];
}

int HeightMap::get_worldspace(const Coordinate& loc) const {
    return get(loc.x - _base_pt.x, loc.z - _base_pt.z);
}

void HeightMap::fill_coord(Coordinate& out) const {
    out.y = get_worldspace(out);
}

int HeightMap::x_len() const { return this->_x_len; }

int HeightMap::z_len() const { return this->_z_len; }

Coordinate HeightMap::base_pt() const { return this->_base_pt.clone(); }

} // namespace mcpp
