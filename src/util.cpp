#include "../include/mcpp/util.h"
#include <cstdlib>
#include <stdexcept>
#include <string>

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

std::ostream& operator<<(std::ostream& out, const Coordinate& coord) {
    out << "(" << coord.x << ", " << coord.y << ", " << coord.z << ")";
    return out;
}

HeightMap::HeightMap(const Coordinate& loc1, const Coordinate& loc2, const std::vector<int>& heights) {
    base_pt = Coordinate {
        std::min(loc1.x, loc2.x),
        0,
        std::min(loc1.z, loc2.z),
    };

    x_len = std::abs(loc1.x - loc2.x) + 1;
    z_len = std::abs(loc1.z - loc2.z) + 1;

    raw_heights = new int[heights.size()];
    std::copy(heights.begin(), heights.end(), raw_heights);
}

int HeightMap::get(int x, int z) const {
    if ((x < 0 || x >= x_len) || (z < 0 || z >= z_len)) {
        throw new std::out_of_range(
            "Out of range access of heightmap at " + std::to_string(x) + "," + std::to_string(z)
            + " (worldspace x=" + std::to_string(base_pt.x + x) + ",z=" + std::to_string(base_pt.z + z));
    }
    // Get 2D from flat vector
    return raw_heights[x * z_len + z];
}

int HeightMap::get_worldspace(const Coordinate& loc) const {
    return get(loc.x - base_pt.x, loc.z - base_pt.z);
}

void HeightMap::fill_coord(Coordinate& out) {
    out.y = get_worldspace(out);
}

} // namespace mcpp
