#include <cmath>
#include <string>
#include <vector>

#include "../include/mcpp/mcpp.h"
#include "connection.h"
#include "util.h"

using namespace std::string_literals;

namespace mcpp {

MinecraftConnection::MinecraftConnection(const std::string& address, uint16_t port) {
  _conn = std::make_unique<SocketConnection>(address, port);
}

MinecraftConnection::~MinecraftConnection() = default;

void MinecraftConnection::postToChat(const std::string& message) {
  _conn->send_command("chat.post", message);
}

void MinecraftConnection::doCommand(const std::string& command) {
  _conn->send_command("player.doCommand", command);
}

void MinecraftConnection::setPlayerPosition(const Coordinate& pos) {
  _conn->send_command("player.setPos", pos.x, pos.y, pos.z);
}

Coordinate MinecraftConnection::getPlayerPosition() const {
  std::string response = _conn->send_receive_command("player.getPos", "");
  std::vector<int32_t> parsed;
  split_response(response, parsed);
  return {parsed[0], parsed[1], parsed[2]};
}

void MinecraftConnection::setPlayerTilePosition(const Coordinate& tile) {
  Coordinate new_tile = tile;
  new_tile.y++;
  setPlayerPosition(new_tile);
}

Coordinate MinecraftConnection::getPlayerTilePosition() const {
  Coordinate player_tile = getPlayerPosition();
  player_tile.y--;
  return player_tile;
}

void MinecraftConnection::setBlock(const Coordinate& loc, const BlockType& block_type) {
  // Static cast required because of stupid ss default of uint8_t as char
  _conn->send_command("world.setBlock", loc.x, loc.y, loc.z, static_cast<int>(block_type.id),
                      static_cast<int>(block_type.mod));
}

void MinecraftConnection::setBlocks(const Coordinate& loc1, const Coordinate& loc2,
                                    const BlockType& block_type) {
  auto [x1, y1, z1] = loc1;
  auto [x2, y2, z2] = loc2;
  _conn->send_command("world.setBlocks", x1, y1, z1, x2, y2, z2, static_cast<int>(block_type.id),
                      static_cast<int>(block_type.mod));
}

BlockType MinecraftConnection::getBlock(const Coordinate& loc) const {
  std::string return_str =
      _conn->send_receive_command("world.getBlockWithData", loc.x, loc.y, loc.z);
  std::vector<uint8_t> parsed;
  split_response(return_str, parsed);

  // Values are id and mod
  return {parsed[0], parsed[1]};
}

Chunk MinecraftConnection::getBlocks(const Coordinate& loc1, const Coordinate& loc2) const {
  std::string response = _conn->send_receive_command("world.getBlocksWithData", loc1.x, loc1.y,
                                                     loc1.z, loc2.x, loc2.y, loc2.z);

  // Received in format 1,2;1,2;1,2 where 1,2 is a block of type 1 and mod 2
  std::vector<BlockType> result;
  std::stringstream stream(response);

  // uint16_t because stupid << is overloaded to read first character instead
  // of number for uint8_t raaaa
  // This shouldn't return anything larger than a uint8_t anyway
  uint16_t id;
  uint16_t mod;
  char delimiter;
  while (stream >> id) {
    stream >> delimiter;
    if (delimiter == ',') {
      stream >> mod;
      result.emplace_back(id, mod);
      stream >> delimiter;
    }
    if (delimiter == ';') {
      continue;
    }
    if (delimiter == EOF) {
      break;
    }
  }

  return Chunk{loc1, loc2, result};
}

int MinecraftConnection::getHeight(int x, int z) const {
  std::string response = _conn->send_receive_command("world.getHeight", x, z);
  return stoi(response);
}

HeightMap MinecraftConnection::getHeights(const Coordinate& loc1, const Coordinate& loc2) const {
  std::string response =
      _conn->send_receive_command("world.getHeights", loc1.x, loc1.z, loc2.x, loc2.z);

  // Returned in format "1,2,3,4,5"
  std::vector<int16_t> parsed;
  split_response(response, parsed);

  return {loc1, loc2, parsed};
}

} // namespace mcpp
