#include "../include/mcpp/mcpp.h"

#include <cmath>
#include <string>
#include <vector>

using std::string_view;
using namespace std::string_literals;
using namespace mcpp;

namespace mcpp {

void splitCommaStringToInts(const std::string& str, std::vector<int>& vec) {
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, ',')) {
        // Fixes flooring issue w/ negative coordinates
        double itemDouble = std::stod(item);
        int itemFloored = static_cast<int>(std::floor(itemDouble));
        vec.push_back(itemFloored);
    }
}

MinecraftConnection::MinecraftConnection(const std::string& address, int port) {
    conn = std::make_unique<SocketConnection>(address, port);
}

void MinecraftConnection::postToChat(const std::string& message) {
    conn->sendCommand("chat.post", message);
}

void MinecraftConnection::doCommand(const std::string& command) {
    conn->sendCommand("player.doCommand", command);
}

void MinecraftConnection::setPlayerPosition(const Coordinate& pos) {
    conn->sendCommand("player.setPos", pos.x, pos.y, pos.z);
}

Coordinate MinecraftConnection::getPlayerPosition() {
    std::string returnString = conn->sendReceiveCommand("player.getPos", "");
    std::vector<int> parsedInts;
    splitCommaStringToInts(returnString, parsedInts);
    return Coordinate(parsedInts[0], parsedInts[1], parsedInts[2]);
}

void MinecraftConnection::setPlayerTilePosition(const Coordinate& tile) {
    Coordinate newTile = tile;
    newTile.y++;
    setPlayerPosition(newTile);
}

Coordinate MinecraftConnection::getPlayerTilePosition() {
    Coordinate playerTile = getPlayerPosition();
    playerTile.y--;
    return playerTile;
}

void MinecraftConnection::setBlock(const Coordinate& loc,
                                   const BlockType& blockType) {
    conn->sendCommand("world.setBlock", loc.x, loc.y, loc.z, blockType.id,
                      blockType.mod);
}

void MinecraftConnection::setBlocks(const Coordinate& loc1,
                                    const Coordinate& loc2,
                                    const BlockType& blockType) {
    auto [x, y, z] = loc1;
    auto [x2, y2, z2] = loc2;
    conn->sendCommand("world.setBlocks", x, y, z, x2, y2, z2, blockType.id,
                      blockType.mod);
}

BlockType MinecraftConnection::getBlock(const Coordinate& loc) {
    std::string returnString =
        conn->sendReceiveCommand("world.getBlockWithData", loc.x, loc.y, loc.z);
    std::vector<int> parsedInts;
    splitCommaStringToInts(returnString, parsedInts);

    // Values are id and mod
    return {parsedInts[0], parsedInts[1]};
}

Chunk MinecraftConnection::getBlocks(const Coordinate& loc1,
                                     const Coordinate& loc2) {
    std::string returnValue =
        conn->sendReceiveCommand("world.getBlocksWithData", loc1.x, loc1.y,
                                 loc1.z, loc2.x, loc2.y, loc2.z);

    // Received in format 1,2;1,2;1,2 where 1,2 is a block of type 1 and mod 2
    std::vector<BlockType> result;
    std::stringstream stream(returnValue);

    int id;
    int data;
    char delimiter;
    while (stream >> id) {
        stream >> delimiter;
        if (delimiter == ',') {
            stream >> data;
            result.emplace_back(id, data);
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

int MinecraftConnection::getHeight(Coordinate2D loc) {
    std::string returnValue =
        conn->sendReceiveCommand("world.getHeight", loc.x, loc.z);
    return stoi(returnValue);
}

const HeightMap MinecraftConnection::getHeights(const Coordinate2D& loc1,
                                                const Coordinate2D& loc2) {
    std::string returnValue = conn->sendReceiveCommand(
        "world.getHeights", loc1.x, loc1.z, loc2.x, loc2.z);

    // Returned in format "1,2,3,4,5"
    std::vector<int> returnVector;
    splitCommaStringToInts(returnValue, returnVector);

    return HeightMap(loc1, loc2, returnVector);
}

} // namespace mcpp
