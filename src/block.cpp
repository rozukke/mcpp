#include "block.h"

//this compares the data as well!!
bool BlockType::operator==(const BlockType &other) const {
    return this->id == other.id && this->data == other.data;
}

BlockType BlockType::withData(int newData) const {
    return BlockType(this->id, newData);
}
