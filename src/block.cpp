#include "../include/mcpp/block.h"

/**
 * Watch out as this also compares the BlockType.id element of the block, so some equalities may behave in
 * somewhat unexpected ways e.g. rotated stairs
 */
bool BlockType::operator==(const BlockType &other) const {
    return this->id == other.id && this->data == other.data;
}

/**
 * Returns a new BlockType object with the specified data. Useful when providing a BlockType to a
 * function that accepts them to quickly modify.
 * @param newData Integer representing the data for the BlockType
 * @return New BlockType object with the specified data
 */
BlockType BlockType::withData(int newData) const {
    return {this->id, newData};
}
