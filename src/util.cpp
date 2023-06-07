#include "../include/mcpp/util.h"

#include <cstdlib>

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


    std::vector<std::vector<std::vector<BlockType>>>
    unflattenBlocksArray(const Coordinate& loc1, const Coordinate& loc2, const std::vector<BlockType>& inVector)
    {
        //initialise empty vector of correct size and shape
        int y_len = abs(loc2.y - loc1.y) + 1;
        int x_len = abs(loc2.x - loc1.x) + 1;
        int z_len = abs(loc2.z - loc1.z) + 1;
        std::vector<std::vector<std::vector<BlockType>>> returnVector (
            y_len, 
            std::vector<std::vector<BlockType>> (
                x_len, 
                std::vector<BlockType> (
                    z_len,
                    BlockType(0)
                )
            )
        );

        //fill
        int index = 0;
        for (int y=0; y < y_len; y++) {
            for (int x=0; x < x_len; x++) {
                for (int z=0; z < z_len; z++) 
                {
                    returnVector[y][x][z] = inVector[index];
                    index++;
                }
            }
        }

        return returnVector;        
    }

    std::vector<std::vector<int>>
    unflattenHeightsArray(const Coordinate& loc1, const Coordinate& loc2, const std::vector<int>& inVector)
    {
        //initialise empty vector of correct size and shape
        int x_len = abs(loc2.x - loc1.x) + 1;
        int z_len = abs(loc2.z - loc1.z) + 1;

        std::vector<std::vector<int>> returnVector (
            x_len,
            std::vector<int> (
                z_len,
                0
            )
        );

        int index = 0;
        for (int x=0; x < x_len; x++) {
            for (int z=0; z < z_len; z++)
            {
                returnVector[x][z] = inVector[index];
                index++;
            }
        }

        return returnVector;
    }
}