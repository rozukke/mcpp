#pragma once

#include <string_view>
#include "connection.h"
#include "block.h"
#include "util.h"

namespace mcpp {
    class MinecraftConnection {
    private:
        std::unique_ptr<SocketConnection> conn;
    public:
        explicit MinecraftConnection(const std::string& address = "localhost", int port = 4711);

        void postToChat(const std::string& message);

        void setSetting(const std::string& setting);

        void doCommand(const std::string& command);

        void setPlayerPosition(const Coordinate& pos);

        Coordinate getPlayerPosition();

        void setBlock(const Coordinate& loc, const BlockType& blockType);

        void setBlocks(const Coordinate& loc1, const Coordinate& loc2, const BlockType& blockType);

        BlockType getBlock(const Coordinate& loc);

        BlockType getBlockWithData(const Coordinate& loc);

        std::vector<BlockType> getBlocks(const Coordinate& loc1, const Coordinate& loc2);

        std::vector<BlockType> getBlocksWithData(const Coordinate& loc1, const Coordinate& loc2);

        int getHeight(int x, int z);

        std::vector<int> getHeights(const Coordinate& loc1, const Coordinate& loc2);

    };
}