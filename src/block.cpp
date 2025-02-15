#include "../include/mcpp/block.h"

namespace mcpp {
bool BlockType::operator==(const BlockType& other) const {
  return this->id == other.id && this->mod == other.mod;
}

bool BlockType::operator!=(const BlockType& other) const { return !(*this == other); }

BlockType BlockType::with_mod(uint8_t modifier) const { return {this->id, modifier}; }

std::ostream& operator<<(std::ostream& out, const BlockType& block) {
  using std::to_string;
  out << "[" << to_string(block.id) << ", " << to_string(block.mod) << "]";
  return out;
}
} // namespace mcpp
