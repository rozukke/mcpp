#include "mcpp.h"
#include "connection.h"
#include <string_view>
#include <string>

using std::string_view;
using namespace std::string_literals;
using namespace mcpp;

namespace mcpp {
    MinecraftConnection::MinecraftConnection(std::string_view address, int port) {
        SocketConnection newConnection(address, port);
        this->conn = newConnection;
    }

    void MinecraftConnection::postToChat(string_view message) {
        conn.sendCommand("chat.post", message);
    }

    void MinecraftConnection::setSetting(std::string_view setting) {
        conn.sendCommand("world.setting", setting);
    }

    void MinecraftConnection::setBlock(Coordinate loc, BlockType blockType) {
        conn.sendCommand("world.setBlock", loc.x, loc.y, loc.z, blockType.id, blockType.data);
    }

//    void MinecraftConnection::setBlock(int x, int y, int z, BlockType blockType) {
//        conn.sendCommand("world.setBlock", x, y, z, blockType.id, blockType.data);
//    }
//
//    BlockType MinecraftConnection::getBlock(int x, int y, int z) {
//        std::string_view returnValue = conn.sendReceiveCommand("world.getBlock", x, y, z);
//        return BlockType(std::stoi(returnValue.data()));
//    }

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

    int MinecraftConnection::getHeight(int x, int z) {
        std::string returnValue = conn.sendReceiveCommand("world.getHeight", x, z);
        return stoi(returnValue);
    }
}