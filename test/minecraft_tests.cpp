#include "../include/mcpp/mcpp.h"
#include "doctest.h"

// Set to 1 if testing with joined player on server
#define PLAYER_TEST 0

using namespace std::string_literals;
using namespace mcpp;

// You may need to set the address to your $(hostname).local if running on WSL2.
SocketConnection tcp_conn("localhost", 4711);
MinecraftConnection mc;

/*
 * All tests require a running instance of Spigot server with the ELCI Legacy
 * plugin in order to run successfully. This requirement stems from the fact
 * that it's a pain in the ass to run a local TCP server just in order to test
 * if the client is sending across a string successfully, and would probably
 * require either writing or importing that TCP server from somewhere and
 * writing around it in a way that does not block code execution when waiting
 * for a connection.
 */

// Run test_suite profile to perform tests in this file.
TEST_CASE("Socket connection test") {
    SUBCASE("Test send") {
        // More or less manual test case used more so to check for errors
        // sending
        tcp_conn.send("chat.post(test string)\n");
        tcp_conn.send("player.setTile(100,100,100)\n");
    }

    SUBCASE("Test receive") {
        tcp_conn.send("world.setBlock(100,100,100,30)\n");
        tcp_conn.send("world.getBlock(100,100,100)\n");
        std::string return_str = tcp_conn.recv();
        CHECK_EQ(return_str, "30");
    }

    SUBCASE("Repeated receive") {
        tcp_conn.send("world.setBlock(100,100,100,29)\n");
        tcp_conn.send("world.getBlock(100,100,100)\n");
        std::string return_str = tcp_conn.recv();
        CHECK_EQ(return_str, "29");
    }

    SUBCASE("Send command") {
        tcp_conn.sendCommand("chat.post", "test message");
    }

    SUBCASE("Send receive command") {
        tcp_conn.sendCommand("world.setBlock", 100, 100, 100, 30);
        tcp_conn.sendCommand("world.setBlock", 100, 100, 100, 26);
        auto result =
            tcp_conn.sendReceiveCommand("world.getBlock", 100, 100, 100);
        CHECK_EQ(result, "26");

        tcp_conn.sendCommand("world.setBlock", 100, 100, 100, 25);
        result = tcp_conn.sendReceiveCommand("world.getBlock", 100, 100, 100);
        CHECK_EQ(result, "25");
    }

    SUBCASE("Test receive when response size is divisible by buffer size") {
        // Assuming buffer size is 1024 bytes
        int expectedResponseSize = 4096;

        // Test coordinate 1
        int x1 = 0, y1 = 0, z1 = 0;
        // Test coordinate 2
        int x2 = 31, y2 = 100, z2 = 31;

        tcp_conn.sendCommand("world.setBlocks", x1, y1, z1, x2, y2, z2,
                             Blocks::DIRT.id, Blocks::DIRT.mod);
        std::string result =
            tcp_conn.sendReceiveCommand("world.getHeights", x1, z1, x2, z2);
        int resultSize = result.size();

        CHECK_EQ(resultSize, expectedResponseSize - 1);
    }

    SUBCASE("Check fail condition") {
        CHECK_THROWS(tcp_conn.sendReceiveCommand("failCommand", ""));
    }
}

TEST_CASE("Test the main mcpp class") {
    Coordinate testLoc(100, 100, 100);

    SUBCASE("postToChat") { mc.postToChat("test string"); }

    SUBCASE("setBlock") { mc.setBlock(testLoc, BlockType(50)); }

    SUBCASE("getBlock") {
        mc.setBlock(testLoc, BlockType(34));
        CHECK_EQ(mc.getBlock(testLoc), BlockType(34));
    }

    // Using values from the Blocks struct in block.h beyond this point

    SUBCASE("getBlock with mod") {
        mc.setBlock(testLoc, BlockType(5, 5));
        CHECK_EQ(mc.getBlock(testLoc), BlockType(5, 5));

        mc.setBlock(testLoc, Blocks::LIGHT_BLUE_CONCRETE);
        CHECK_EQ(mc.getBlock(testLoc), Blocks::LIGHT_BLUE_CONCRETE);
    }

    SUBCASE("getHeight") {
        Coordinate heightTestLoc(200, 200, 200);
        mc.setBlock(heightTestLoc, Blocks::DIRT);
        auto height = mc.getHeight(heightTestLoc.x, heightTestLoc.z);
        CHECK_EQ(height, heightTestLoc.y);
    }

    SUBCASE("getHeights") {
        Coordinate platformCoord1(151, 100, 151);
        Coordinate platformCoord2(160, 100, 160);

        // Create even heights
        mc.setBlocks(platformCoord1, platformCoord2, Blocks::DIRT);

        std::vector expected =
            std::vector<std::vector<int>>(10, std::vector<int>(10, 100));

        auto resultHeights = mc.getHeights(platformCoord1, platformCoord2);
        CHECK_EQ(resultHeights, expected);
    }

    SUBCASE("setBlocks") {
        Coordinate loc1{100, 100, 100};
        Coordinate loc2{110, 110, 110};
        mc.setBlocks(loc1, loc2, Blocks::STONE);
    }
}

TEST_CASE("getBlocks and Chunk operations") {
    // Setup
    Coordinate loc1{100, 100, 100};
    Coordinate loc2{110, 110, 110};

    // Reset blocks that existed before
    mc.setBlocks(loc1, loc2, Blocks::AIR);
    mc.setBlocks(loc1, loc2, Blocks::BRICKS);
    mc.setBlock(loc1, Blocks::GOLD_BLOCK);
    mc.setBlock(loc2, Blocks::DIAMOND_BLOCK);
    mc.setBlock(loc1 + Coordinate{1, 2, 3}, Blocks::IRON_BLOCK);
    Chunk res = mc.getBlocks(loc1, loc2);

    SUBCASE("Block accessing returns correct block using get()") {
        CHECK_EQ(res.get(0, 0, 0), Blocks::GOLD_BLOCK);
        CHECK_EQ(res.get(1, 1, 1), Blocks::BRICKS);
        CHECK_EQ(res.get(1, 2, 3), Blocks::IRON_BLOCK);
        CHECK_EQ(res.get(10, 10, 10), Blocks::DIAMOND_BLOCK);
    }

    SUBCASE("Block accessing returns correct block using get_worldspace()") {
        CHECK_EQ(res.get_worldspace(loc1), Blocks::GOLD_BLOCK);
        CHECK_EQ(res.get_worldspace(loc1 + Coordinate{1, 1, 1}),
                 Blocks::BRICKS);
        CHECK_EQ(res.get_worldspace(loc1 + Coordinate{1, 2, 3}),
                 Blocks::IRON_BLOCK);
        CHECK_EQ(res.get_worldspace(loc2), Blocks::DIAMOND_BLOCK);
    }

    SUBCASE("Access out of bounds correctly throws") {
        CHECK_THROWS(res.get(11, 0, 0));
        CHECK_THROWS(res.get(0, 11, 0));
        CHECK_THROWS(res.get(0, 0, 11));
        CHECK_THROWS(res.get(-1, 0, 0));
        CHECK_THROWS(res.get(0, -1, 0));
        CHECK_THROWS(res.get(0, 0, -1));
        CHECK_THROWS(res.get_worldspace(loc1 + Coordinate{-1, -1, -1}));
        CHECK_THROWS(res.get_worldspace(loc1 + Coordinate{11, 11, 11}));
    }
}

// Requires player joined to server, will throw serverside if player is not
// joined and hang execution
#if PLAYER_TEST

TEST_CASE("Player operations") {
    Coordinate testLoc(110, 110, 110);
    mc.setBlock(testLoc, Blocks::DIRT);
    SUBCASE("Execute command") { mc.doCommand("time set noon"); }

    SUBCASE("Set position") {
        mc.setPlayerPosition(testLoc + Coordinate(0, 1, 0));
    }

    SUBCASE("Get position") {
        Coordinate playerLoc = mc.getPlayerPosition();
        CHECK((playerLoc == (testLoc + Coordinate(0, 1, 0))));
    }

    SUBCASE("Check correct flooring") {
        Coordinate negativeLoc(-2, 100, -2);
        mc.doCommand("tp -2 100 -2");
        CHECK_EQ(mc.getPlayerPosition(), negativeLoc);
    }
}

#endif

TEST_CASE("Test blocks struct") {
    Coordinate testLoc;
    mc.setBlock(testLoc, Blocks::AIR);
    CHECK_EQ(mc.getBlock(testLoc), Blocks::AIR);
    mc.setBlock(testLoc, Blocks::STONE);
    CHECK_EQ(mc.getBlock(testLoc), Blocks::STONE);
}
