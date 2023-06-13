#pragma once

#include <string_view>
#include <memory>
#include <vector>
#include "connection.h"
#include "block.h"
#include "util.h"

namespace mcpp {
    class MinecraftConnection {
    private:
        std::unique_ptr<SocketConnection> conn;

        static std::vector<std::vector<std::vector<BlockType>>>
        unflattenBlocksArray(const Coordinate& loc1,
                             const Coordinate& loc2,
                             const std::vector<BlockType>& inVector);

        static std::vector<std::vector<int>>
        unflattenHeightsArray(const Coordinate& loc1,
                              const Coordinate& loc2,
                              const std::vector<int>& inVector);

    public:
        /**
         * Represents the main endpoint for interaction with the minecraft world.
         * @param address String address in IPV4 format, defaults to "localhost"
         * @param port Integer port to run on, defaults to 4711 as that is the port for ELCI
         */
        explicit MinecraftConnection(const std::string& address = "localhost",
                                     int port = 4711);

        /**
         * Sends a message to the in-game chat, does not require a joined player
         * @param message
         */
        void postToChat(const std::string& message);

        void setSetting(const std::string& setting);

        /**
         * Performs an in-game minecraft command. Players have to exist on the server and
         * should be server operators (default with ELCI)
         * @param command Command string in the in-game format (e.g. "time set day")
         */
        void doCommand(const std::string& command);

        void setPlayerPosition(const Coordinate& pos);

        Coordinate getPlayerPosition();

        /**
         * Sets block at Coordinate loc to the BlockType specified by blockType
         * @param loc
         * @param blockType
         */
        void setBlock(const Coordinate& loc, const BlockType& blockType);

        /**
         * Sets a cuboid of blocks to the specified BlockType blockType, with the corners of the cuboid
         * provided by the Coordinate loc1 and loc2
         * @param loc1
         * @param loc2
         * @param blockType
         */
        void setBlocks(const Coordinate& loc1, const Coordinate& loc2,
                       const BlockType& blockType);

        /**
         * Returns BlockType object from the specified Coordinate loc with modifier
         * @param loc
         * @return BlockType of the requested block
         */
        BlockType getBlock(const Coordinate& loc);

        /**
         * Returns a 3D vector of the BlockTypes of the requested cuboid with modifiers
         * @param loc1 1st corner of the cuboid
         * @param loc2 2nd corner of the cuboid
         * @return 3D vector of BlockType in the specified cuboid.
         */
        std::vector<std::vector<std::vector<BlockType>>>
        getBlocks(const Coordinate& loc1, const Coordinate& loc2);

        /**
         * IMPORTANT: DO NOT USE FOR LARGE AREAS, IT WILL BE VERY SLOW
         * USE getHeights() INSTEAD
         * Gets the y-value of the highest non-air block at the specified (x, y) coordinate.
         * @param x
         * @param z
         * @return Returns the integer y-height at the requested coordinate.
         */
        int getHeight(int x, int z);

        /**
         * Provides a scaled option of the getHeight call to allow for considerable performance gains. USE THIS
         * instead of getHeight in a for loop.
         * @param loc1
         * @param loc2
         * @return Returns a vector of integers representing the 2D area of heights.
         */
        std::vector<std::vector<int>>
        getHeights(const Coordinate& loc1, const Coordinate& loc2);

    };
}