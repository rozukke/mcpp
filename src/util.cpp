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
    this->base_pt = min.clone();

    Coordinate dim = loc1 - loc2;
    x_len = std::abs(dim.x) + 1;
    y_len = std::abs(dim.y) + 1;
    z_len = std::abs(dim.z) + 1;

    this->raw_data = new BlockType[block_list.size()];
    std::copy(block_list.begin(), block_list.end(), raw_data);
}

Chunk::~Chunk() { delete[] raw_data; }

BlockType Chunk::get(int x, int y, int z) {
    if ((x < 0 || y < 0 || z < 0) ||
        (x > x_len - 1 || y > y_len - 1 || z > z_len - 1)) {
        throw std::out_of_range("Out of bounds Chunk access at " +
                                to_string(Coordinate(x, y, z)));
    }
    return raw_data[z + z_len * (x + y_len * y)];
}

BlockType Chunk::get_worldspace(const Coordinate& pos) {
    Coordinate array_pos = pos - base_pt;
    if ((array_pos.x < 0 || array_pos.y < 0 || array_pos.z < 0) ||
        (array_pos.x > x_len - 1 || array_pos.y > y_len - 1 ||
         array_pos.z > z_len - 1)) {
        throw std::out_of_range("Out of bounds Chunk access at " +
                                to_string(array_pos) + " (world coordinate " +
                                to_string(pos) + " )");
    }
    return raw_data[array_pos.z + z_len * (array_pos.x + y_len * array_pos.y)];
}

} // namespace mcpp
