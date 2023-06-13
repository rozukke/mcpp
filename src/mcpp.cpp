#include "../include/mcpp/mcpp.h"
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
            vec.push_back(std::stoi(item));
        }
    }

    MinecraftConnection::MinecraftConnection(const std::string& address,
                                             int port) {
        conn = std::make_unique<SocketConnection>(address, port);
    }

    void MinecraftConnection::postToChat(const std::string& message) {
        conn->sendCommand("chat.post", message);
    }

    void MinecraftConnection::setSetting(const std::string& setting) {
        conn->sendCommand("world.setting", setting);
    }

    void MinecraftConnection::doCommand(const std::string& command) {
        conn->sendCommand("player.doCommand", command);
    }

    void MinecraftConnection::setPlayerPosition(const Coordinate& pos) {
        conn->sendCommand("player.setPos", pos.x, pos.y, pos.z);
    }

    Coordinate MinecraftConnection::getPlayerPosition() {
        // TODO remove currently required empty string that ensures formatting
        std::string returnString = conn->sendReceiveCommand("player.getPos", "");
        std::vector<int> parsedInts;
        splitCommaStringToInts(returnString, parsedInts);
        return Coordinate(parsedInts[0], parsedInts[1], parsedInts[2]);
    }

    // TODO tile variants of getPos and setPos


    void MinecraftConnection::setBlock(const Coordinate& loc,
                                       const BlockType& blockType) {
        conn->sendCommand("world.setBlock", loc.x, loc.y, loc.z, blockType.id,
                          blockType.data);
    }


    void MinecraftConnection::setBlocks(const Coordinate& loc1,
                                        const Coordinate& loc2,
                                        const BlockType& blockType) {
        auto [x, y, z] = loc1;
        auto [x2, y2, z2] = loc2;
        conn->sendCommand("world.setBlocks", x, y, z, x2, y2, z2, blockType.id,
                          blockType.data);
    }


    BlockType MinecraftConnection::getBlock(const Coordinate& loc) {
        std::string returnValue = conn->sendReceiveCommand("world.getBlock",
                                                           loc.x, loc.y, loc.z);
        return {std::stoi(returnValue)};
    }


    BlockType MinecraftConnection::getBlockWithData(const Coordinate& loc) {
        std::string returnString = conn->sendReceiveCommand(
                "world.getBlockWithData", loc.x, loc.y, loc.z);
        std::vector<int> parsedInts;
        splitCommaStringToInts(returnString, parsedInts);

        // Data and id
        return {parsedInts[0], parsedInts[1]};
    }


    std::vector<std::vector<std::vector<BlockType>>>
    MinecraftConnection::getBlocks(const Coordinate& loc1,
                                   const Coordinate& loc2) {
        std::string returnValue = conn->sendReceiveCommand("world.getBlocks",
                                                           loc1.x, loc1.y,
                                                           loc1.z,
                                                           loc2.x, loc2.y,
                                                           loc2.z);
        std::stringstream ss(returnValue);
        std::string container;
        std::vector<BlockType> returnVector;

        while (std::getline(ss, container, ',')) {
            returnVector.emplace_back(std::stoi(container));
        }

        return unflattenBlocksArray(loc1, loc2, returnVector);
    }


    std::vector<std::vector<std::vector<BlockType>>>
    MinecraftConnection::getBlocksWithData(
            const Coordinate& loc1, const Coordinate& loc2) {
        std::string returnValue = conn->sendReceiveCommand(
                "world.getBlocksWithData",
                loc1.x, loc1.y, loc1.z,
                loc2.x, loc2.y, loc2.z);

        // Received in format 1,2;1,2;1,2 where 1,2 is a block of type 1 and data 2
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
        return unflattenBlocksArray(loc1, loc2, result);
    }


    int MinecraftConnection::getHeight(int x, int z) {
        std::string returnValue = conn->sendReceiveCommand("world.getHeight", x,
                                                           z);
        return stoi(returnValue);
    }


    std::vector<std::vector<int>>
    MinecraftConnection::getHeights(const Coordinate& loc1,
                                    const Coordinate& loc2) {
        std::string returnValue = conn->sendReceiveCommand("world.getHeights",
                                                           loc1.x, loc1.z,
                                                           loc2.x, loc2.z);

        // Returned in format "1,2,3,4,5"
        std::vector<int> returnVector;
        splitCommaStringToInts(returnValue, returnVector);

        return unflattenHeightsArray(loc1, loc2, returnVector);
    }


    std::vector<std::vector<std::vector<BlockType>>>
    MinecraftConnection::unflattenBlocksArray(const Coordinate& loc1,
                                              const Coordinate& loc2,
                                              const std::vector<BlockType>& inVector) {
        // Initialise empty vector of correct size and shape
        int y_len = abs(loc2.y - loc1.y) + 1;
        int x_len = abs(loc2.x - loc1.x) + 1;
        int z_len = abs(loc2.z - loc1.z) + 1;
        std::vector<std::vector<std::vector<BlockType>>> returnVector(
                y_len,
                std::vector<std::vector<BlockType>>(
                        x_len,
                        std::vector<BlockType>(
                                z_len,
                                BlockType(0)
                        )
                )
        );

        int index = 0;
        for (int y = 0; y < y_len; y++) {
            for (int x = 0; x < x_len; x++) {
                for (int z = 0; z < z_len; z++) {
                    returnVector[y][x][z] = inVector[index];
                    index++;
                }
            }
        }

        return returnVector;
    }

    std::vector<std::vector<int>>
    MinecraftConnection::unflattenHeightsArray(const Coordinate& loc1, const Coordinate& loc2,
                                               const std::vector<int>& inVector) {
        // Initialise empty vector of correct size and shape
        int x_len = abs(loc2.x - loc1.x) + 1;
        int z_len = abs(loc2.z - loc1.z) + 1;

        std::vector<std::vector<int>> returnVector(
                x_len,
                std::vector<int>(
                        z_len,
                        0
                )
        );

        int index = 0;
        for (int x = 0; x < x_len; x++) {
            for (int z = 0; z < z_len; z++) {
                returnVector[x][z] = inVector[index];
                index++;
            }
        }

        return returnVector;
    }
}