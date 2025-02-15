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

std::string to_string(const Coordinate& coord) {
  using std::to_string;
  return "(" + to_string(coord.x) + "," + to_string(coord.y) + "," + to_string(coord.z) + ")";
}

std::ostream& operator<<(std::ostream& out, const Coordinate& coord) {
  out << to_string(coord);
  return out;
}
} // namespace mcpp
