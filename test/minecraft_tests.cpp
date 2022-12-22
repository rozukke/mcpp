#include "doctest.h"
#include "../src/connection.h"
#include "../src/mcpp.h"
#include "../src/util.h"
#include "../src/block.h"

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

    SUBCASE("setBlocks") {
        Coordinate testLoc2(96, 96, 96);
        mc.setBlocks(testLoc, testLoc2, Blocks::STONE);
    }

    SUBCASE("getBlocks") {
        Coordinate testLoc2(96, 96, 96);
        mc.setBlocks(testLoc, testLoc2, Blocks::DIRT);

        std::vector<BlockType> expected(125, BlockType(3));
        std::vector returnVector = mc.getBlocks(testLoc, testLoc2);
        CHECK((returnVector == expected));
    }
}

TEST_CASE("Test blocks struct") {
    MinecraftConnection mc;
    Coordinate testLoc;
    mc.setBlock(testLoc, Blocks::AIR);
    CHECK((mc.getBlock(testLoc) == Blocks::AIR));
    mc.setBlock(testLoc, Blocks::STONE);
    CHECK((mc.getBlock(testLoc) == Blocks::STONE));
}