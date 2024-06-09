#include "../include/mcpp/block.h"

namespace mcpp {
    BlockType::BlockType(): id(0), mod(0) {}
    
    bool BlockType::operator==(const BlockType& other) const {
        return this->id == other.id && this->mod == other.mod;
    }

    bool BlockType::operator!=(const BlockType& other) const {
        return !(*this == other);
    }

    BlockType BlockType::withMod(int modifier) const {
        return {this->id, modifier};
    }

    std::ostream& operator<< (std::ostream& out, const BlockType& block) {
        out << "[" << block.id << ", " << block.mod << "]";
        return out;
    }
}
