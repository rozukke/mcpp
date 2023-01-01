#pragma once

#include <string_view>
#include "connection.h"
#include "block.h"
#include "util.h"

namespace mcpp {
    class MinecraftConnection {
    private:
        SocketConnection conn;
    public:
        explicit MinecraftConnection(std::string_view address = "localhost", int port = 4711);

        void postToChat(std::string_view message);

        void setSetting(std::string_view setting);

        void doCommand(std::string_view command);

        void setPlayerPosition(Coordinate pos);

        Coordinate getPlayerPosition();

        void setBlock(Coordinate loc, BlockType blockType);

        void setBlocks(Coordinate loc1, Coordinate loc2, BlockType blockType);

        BlockType getBlock(Coordinate loc);

        BlockType getBlockWithData(Coordinate loc);

        std::vector<BlockType> getBlocks(Coordinate loc1, Coordinate loc2);

        std::vector<BlockType> getBlocksWithData(Coordinate loc1, Coordinate loc2);

        int getHeight(int x, int z);

        std::vector<int> getHeights(Coordinate loc1, Coordinate loc2);

    };
}