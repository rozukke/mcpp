#include "../include/mcpp.h"
#include "../include/connection.h"
#include <string_view>
#include <string>
#include <utility>

using std::string_view;
using namespace std::string_literals;
using namespace mcpp;

namespace mcpp {
    MinecraftConnection::MinecraftConnection(std::string_view address, int port) {
        SocketConnection newConnection(address, port);
        conn = newConnection;
    }

    void MinecraftConnection::postToChat(string_view message) {
        conn.sendCommand("chat.post", message);
    }

    void MinecraftConnection::setSetting(std::string_view setting) {
        conn.sendCommand("world.setting", setting);
    }

    void MinecraftConnection::doCommand(std::string_view command) {
        conn.sendCommand("player.doCommand", command);
    }

    void MinecraftConnection::setBlock(Coordinate loc, BlockType blockType) {
        conn.sendCommand("world.setBlock", loc.x, loc.y, loc.z, blockType.id, blockType.data);
    }

    void MinecraftConnection::setBlocks(Coordinate loc1, Coordinate loc2, BlockType blockType) {
        auto [x, y, z] = loc1;
        auto [x2, y2, z2] = loc2;
        conn.sendCommand("world.setBlocks", x, y, z, x2, y2, z2, blockType.id, blockType.data);
    }

    BlockType MinecraftConnection::getBlock(Coordinate loc) {
        std::string returnValue = conn.sendReceiveCommand("world.getBlock", loc.x, loc.y, loc.z);
        return {std::stoi(returnValue)};
    }

    BlockType MinecraftConnection::getBlockWithData(Coordinate loc) {
        std::string returnValue = conn.sendReceiveCommand("world.getBlockWithData", loc.x, loc.y, loc.z);
        auto commaLoc = returnValue.find(',');

        std::string id = returnValue.substr(0, commaLoc);
        returnValue.erase(0, commaLoc + 1);
        std::string data = returnValue;

        return {std::stoi(id), std::stoi(data)};
    }

    std::vector<BlockType> MinecraftConnection::getBlocks(Coordinate loc1, Coordinate loc2) {
        std::string returnValue = conn.sendReceiveCommand("world.getBlocks",
                                                          loc1.x, loc1.y, loc1.z,
                                                          loc2.x, loc2.y, loc2.z);
        std::stringstream ss(returnValue);
        std::string container;
        std::vector<BlockType> returnVector;

        while (std::getline(ss, container, ',')) {
            returnVector.emplace_back(BlockType(std::stoi(container)));
        }

        return returnVector;
    }

    std::vector<BlockType> MinecraftConnection::getBlocksWithData(Coordinate loc1, Coordinate loc2) {
        std::string returnValue = conn.sendReceiveCommand("world.getBlocksWithData",
                                                          loc1.x, loc1.y, loc1.z,
                                                          loc2.x, loc2.y, loc2.z);

        // received in format 1,2;1,2;1,2 where 1,2 is a block of type 1 and data 2
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
        return result;
    }

    int MinecraftConnection::getHeight(int x, int z) {
        std::string returnValue = conn.sendReceiveCommand("world.getHeight", x, z);
        return stoi(returnValue);
    }

    std::vector<int> MinecraftConnection::getHeights(Coordinate loc1, Coordinate loc2) {
        std::string returnValue = conn.sendReceiveCommand("world.getHeights", loc1.x, loc1.z, loc2.x, loc2.z);

        // Returned in format "1,2,3,4,5"
        std::stringstream ss(returnValue);
        std::string container;
        std::vector<int> returnVector;

        while (std::getline(ss, container, ',')) {
            returnVector.emplace_back(std::stoi(container));
        }

        return returnVector;
    }
}