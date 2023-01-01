#include "../include/mcpp/util.h"

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

    Coordinate Coordinate::operator+(const Coordinate &obj) const {
        Coordinate result;
        result.x = this->x + obj.x;
        result.y = this->y + obj.y;
        result.z = this->z + obj.z;
        return result;
    }

    bool Coordinate::operator==(const Coordinate &obj) const {
        return (this->x == obj.x) && (this->y == obj.y) && (this->z == obj.z);
    }

    Coordinate Coordinate::operator-(const Coordinate &obj) const {
        Coordinate result;
        result.x = this->x - obj.x;
        result.y = this->y - obj.y;
        result.z = this->z - obj.z;
        return result;
    }

    Coordinate Coordinate::clone() const {
        return Coordinate(this->x, this->y, this->z);
    }
}