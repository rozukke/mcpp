#include "../include/mcpp/coordinate.h"
#include <string>

namespace mcpp {
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

bool Coordinate::operator!=(const Coordinate& obj) const { return !(*this == obj); }

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

  size_t nx = obj.x - lower;
  size_t ny = obj.y - lower;
  size_t nz = obj.z - lower;

  return nx * base * base + ny * base + nz;
}

std::string to_string(const Coordinate& coord) {
  using std::to_string;
  return "(" + to_string(coord.x) + "," + to_string(coord.y) + "," + to_string(coord.z) + ")";
}

std::ostream& operator<<(std::ostream& out, const Coordinate& coord) {
  out << to_string(coord);
  return out;
}
} // namespace mcpp
