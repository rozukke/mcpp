#include "doctest.h"
#include "../include/mcpp/connection.h"
#include "../include/mcpp/mcpp.h"
#include "../include/mcpp/util.h"
#include "../include/mcpp/block.h"


//set to 1 if testing with joined player on server
#define PLAYERTEST 1

using std::string;
using namespace std::string_literals;
using namespace mcpp;

/*
 * All tests require a running instance of Spigot server with the ELCI Legacy plugin in order to run successfully. This
 * requirement stems from the fact that it's a pain in the ass to run a local TCP server just in order to test if the
 * client is sending across a string successfully, and would probably require either writing or importing that
 * TCP server from somewhere and writing around it in a way that does not block code execution when waiting for a
 * connection.
 */



//Run test_suite profile to perform tests in this file.
TEST_CASE("Socket connection test")
{
    SocketConnection tcp_conn;

    SUBCASE("Test send") {
        //more or less manual test case used more so to check for errors sending
        tcp_conn.send("chat.post(test string)\n");
        tcp_conn.send("player.setTile(100,100,100)\n");
    }

    SUBCASE("Test receive") {
        tcp_conn.send("world.setBlock(100,100,100,30)\n");
        tcp_conn.send("world.getBlock(100,100,100)\n");
        string return_str = tcp_conn.recv();
        CHECK((return_str == "30"));
    }

    SUBCASE("Repeated receive") {
        tcp_conn.send("world.setBlock(100,100,100,29)\n");
        tcp_conn.send("world.getBlock(100,100,100)\n");
        string return_str = tcp_conn.recv();
        CHECK((return_str == "29"));
    }

    SUBCASE("Send command") {
        tcp_conn.sendCommand("chat.post", "test message");
    }

    SUBCASE("Send receive command") {
        tcp_conn.sendCommand("world.setBlock", 100, 100, 100, 30);
        tcp_conn.sendCommand("world.setBlock", 100, 100, 100, 26);
        auto result = tcp_conn.sendReceiveCommand("world.getBlock", 100, 100, 100);
        CHECK((result == "26"));

        tcp_conn.sendCommand("world.setBlock", 100, 100, 100, 25);
        result = tcp_conn.sendReceiveCommand("world.getBlock", 100, 100, 100);
        CHECK((result == "25"));
    }

    SUBCASE("Check fail condition") {
        CHECK_THROWS(tcp_conn.sendReceiveCommand("failCommand", ""));
    }
}

TEST_CASE("Test the main mcpp class") {
    MinecraftConnection mc;
    Coordinate testLoc(100, 100, 100);

    SUBCASE("postToChat") {
        mc.postToChat("test string");
    }


    SUBCASE("setBlock") {
        mc.setBlock(testLoc, BlockType(50));
    }

    SUBCASE("getBlock") {
        mc.setBlock(testLoc, BlockType(34));
        auto resultBlock = mc.getBlock(testLoc);
        CHECK((mc.getBlock(testLoc) == BlockType(34)));
    }

    //Using values from the Blocks struct in block.h beyond this point

    SUBCASE("getBlockWithData") {
        mc.setBlock(testLoc, BlockType(5, 5));
        CHECK((mc.getBlockWithData(testLoc) == BlockType(5, 5)));

        mc.setBlock(testLoc, Blocks::LIGHT_BLUE_CONCRETE);
        CHECK((mc.getBlockWithData(testLoc) == Blocks::LIGHT_BLUE_CONCRETE));
    }

    SUBCASE("getHeight") {
        Coordinate heightTestLoc(200, 200, 200);
        mc.setBlock(heightTestLoc, Blocks::DIRT);
        auto height = mc.getHeight(heightTestLoc.x, heightTestLoc.z);
        CHECK((height == heightTestLoc.y));
    }

    SUBCASE("getHeights") {
        Coordinate platformCoord1(151, 100, 151);
        Coordinate platformCoord2(160, 100, 160);

        //create even heights
        mc.setBlocks(platformCoord1, platformCoord2, Blocks::DIRT);

        std::vector<int> expected(100, 100);
        auto resultHeights = mc.getHeights(platformCoord1, platformCoord2);
        CHECK((resultHeights == expected));
    }

    // used for cuboid functions
    Coordinate testLoc2(96, 96, 96);

    SUBCASE("setBlocks") {
        mc.setBlocks(testLoc, testLoc2, Blocks::STONE);
    }

    SUBCASE("getBlocks") {
        mc.setBlocks(testLoc, testLoc2, Blocks::DIRT);

        std::vector<BlockType> expected(125, BlockType(3));
        std::vector returnVector = mc.getBlocks(testLoc, testLoc2);
        CHECK((returnVector == expected));
    }

    SUBCASE("getBlocksWithData") {
        mc.setBlocks(testLoc, testLoc2, Blocks::GRANITE);

        std::vector<BlockType> expected(125, Blocks::GRANITE);
        std::vector returnVector = mc.getBlocksWithData(testLoc, testLoc2);

        CHECK((returnVector == expected));
    }
}

//requires player joined to server, will throw serverside if player is not joined and hang execution
#if PLAYERTEST

TEST_CASE("Player operations") {
    MinecraftConnection mc;
    Coordinate testLoc(110, 110, 110);
    mc.setBlock(testLoc, Blocks::DIRT);
    SUBCASE("Execute command") {
        mc.doCommand("time set noon");
    }

    SUBCASE("Set position") {
        mc.setPlayerPosition(testLoc + Coordinate(0, 1, 0));
    }

    SUBCASE("Get position") {
        Coordinate playerLoc = mc.getPlayerPosition();
        CHECK((playerLoc == (testLoc + Coordinate(0, 1, 0))));
    }
}

#endif


TEST_CASE("Test blocks struct") {
    MinecraftConnection mc;
    Coordinate testLoc;
    mc.setBlock(testLoc, Blocks::AIR);
    CHECK((mc.getBlock(testLoc) == Blocks::AIR));
    mc.setBlock(testLoc, Blocks::STONE);
    CHECK((mc.getBlock(testLoc) == Blocks::STONE));
}