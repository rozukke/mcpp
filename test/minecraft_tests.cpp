#include "../include/mcpp/mcpp.h"
#include "doctest.h"

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
    }

    SUBCASE("Test receive") {
        tcp_conn.send("world.setBlock(100,100,100,30)\n");
        tcp_conn.send("world.getBlock(100,100,100)\n");
        std::string return_str = tcp_conn.recv();
        CHECK_EQ(return_str, "30");
        tcp_conn.send("world.setBlock(100,100,100,0)\n");
    }

    SUBCASE("Repeated receive") {
        tcp_conn.send("world.setBlock(100,100,100,29)\n");
        tcp_conn.send("world.getBlock(100,100,100)\n");
        std::string return_str = tcp_conn.recv();
        CHECK_EQ(return_str, "29");
        tcp_conn.send("world.setBlock(100,100,100,0)\n");
    }

    SUBCASE("Send command") {
        tcp_conn.sendCommand("chat.post", "test message");
    }

    SUBCASE("Send receive command") {
        tcp_conn.sendCommand("world.setBlock", 100, 100, 100, 26);
        auto result =
            tcp_conn.sendReceiveCommand("world.getBlock", 100, 100, 100);
        CHECK_EQ(result, "26");

        tcp_conn.sendCommand("world.setBlock", 100, 100, 100, 25);
        result = tcp_conn.sendReceiveCommand("world.getBlock", 100, 100, 100);
        CHECK_EQ(result, "25");
        tcp_conn.sendCommand("world.setBlock", 100, 100, 100, 0);
    }

    /*
     * Validates non-existence of a rare bug where specific response sizes
     * would hang execution.
     */
    SUBCASE("Test receive when response size is divisible by buffer size") {
        // Assuming buffer size is 1024 bytes
        int expected_size = 4096;

        int x1 = 0, y1 = 0, z1 = 0;
        int x2 = 31, y2 = 100, z2 = 31;

        tcp_conn.sendCommand("world.setBlocks", x1, y1, z1, x2, y2, z2,
                             Blocks::DIRT.id, Blocks::DIRT.mod);
        std::string result =
            tcp_conn.sendReceiveCommand("world.getHeights", x1, z1, x2, z2);
        int real_size = result.size();

        // -1 because newline is removed
        CHECK_EQ(real_size, expected_size - 1);

        // Cleanup
        tcp_conn.sendCommand("world.setBlocks", x1, y1, z1, x2, y2, z2,
                             Blocks::AIR.id, Blocks::AIR.mod);
    }

    SUBCASE("Check fail condition") {
        CHECK_THROWS(tcp_conn.sendReceiveCommand("failCommand", ""));
    }
}

TEST_CASE("Test the main mcpp class") {
    Coordinate test_loc(100, 100, 100);

    SUBCASE("postToChat") { mc.postToChat("test string"); }

    SUBCASE("setBlock") { mc.setBlock(test_loc, BlockType(50)); }

    SUBCASE("getBlock") {
        mc.setBlock(test_loc, BlockType(34));
        CHECK_EQ(mc.getBlock(test_loc), BlockType(34));
    }

    // Using values from the Blocks struct in block.h beyond this point
    SUBCASE("getBlock with mod") {
        mc.setBlock(test_loc, BlockType(5, 5));
        CHECK_EQ(mc.getBlock(test_loc), BlockType(5, 5));

        mc.setBlock(test_loc, Blocks::LIGHT_BLUE_CONCRETE);
        CHECK_EQ(mc.getBlock(test_loc), Blocks::LIGHT_BLUE_CONCRETE);
    }

    SUBCASE("getHeight") {
        Coordinate heightTestLoc(300, 200, 300);
        mc.setBlock(heightTestLoc, Blocks::DIRT);
        auto height = mc.getHeight(heightTestLoc.x, heightTestLoc.z);
        CHECK_EQ(height, heightTestLoc.y);

        // Clean up
        mc.setBlock(heightTestLoc, Blocks::AIR);
    }

    SUBCASE("setBlocks") {
        Coordinate loc1{100, 100, 100};
        Coordinate loc2{110, 110, 110};
        mc.setBlocks(loc1, loc2, Blocks::STONE);
    }
}

TEST_CASE("getBlocks and Chunk operations") {
    // Setup
    Coordinate test_loc(100, 100, 100);
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

    mc.setBlock(test_loc, BlockType(0));
}

TEST_CASE("Test blocks struct") {
    Coordinate testLoc;
    mc.setBlock(testLoc, Blocks::AIR);
    CHECK_EQ(mc.getBlock(testLoc), Blocks::AIR);
    mc.setBlock(testLoc, Blocks::STONE);
    CHECK_EQ(mc.getBlock(testLoc), Blocks::STONE);
}

TEST_CASE("HeightMap functionality") {
    // 319 is the build limit in 1.19
    mc.setBlocks(Coordinate{200, 300, 200}, Coordinate{210, 319, 210},
                 Blocks::AIR);
    mc.setBlocks(Coordinate{200, 300, 200}, Coordinate{210, 300, 210},
                 Blocks::STONE);
    mc.setBlock(Coordinate{200, 301, 200}, Blocks::STONE);
    mc.setBlock(Coordinate{210, 301, 210}, Blocks::STONE);
    mc.setBlock(Coordinate{201, 301, 202}, Blocks::STONE);

    SUBCASE("getters") {
        HeightMap data =
            mc.getHeights(Coordinate{200, 0, 200}, Coordinate{210, 0, 210});

        CHECK_EQ(data.base_pt(), Coordinate{200, 0, 200});
        CHECK_EQ(data.x_len(), 11);
        CHECK_EQ(data.z_len(), 11);

        data =
            mc.getHeights(Coordinate{210, 300, 210}, Coordinate{200, 310, 200});

        CHECK_EQ(data.base_pt(), Coordinate{200, 0, 200});
        CHECK_EQ(data.x_len(), 11);
        CHECK_EQ(data.z_len(), 11);
    }

    SUBCASE("get") {
        HeightMap data =
            mc.getHeights(Coordinate{200, 0, 200}, Coordinate{210, 0, 210});
        CHECK_EQ(data.get(0, 0), 301);
        CHECK_EQ(data.get(1, 1), 300);
        CHECK_EQ(data.get(10, 10), 301);
        CHECK_EQ(data.get(1, 2), 301);
    }

    SUBCASE("get_worldspace") {
        HeightMap data =
            mc.getHeights(Coordinate{200, 0, 200}, Coordinate{210, 0, 210});
        CHECK_EQ(data.get_worldspace(Coordinate{200, 0, 200}), 301);
        CHECK_EQ(data.get_worldspace(Coordinate{201, 0, 201}), 300);
        CHECK_EQ(data.get_worldspace(Coordinate{210, 0, 210}), 301);
        CHECK_EQ(data.get_worldspace(Coordinate{201, 0, 202}), 301);
    }

    SUBCASE("fill_coord") {
        HeightMap data =
            mc.getHeights(Coordinate{200, 0, 200}, Coordinate{210, 0, 210});

        Coordinate to_fill{200, 0, 200};
        data.fill_coord(to_fill);
        CHECK_EQ(to_fill.y, 301);
    }

    SUBCASE("Bounds checking") {
        HeightMap data =
            mc.getHeights(Coordinate{200, 0, 200}, Coordinate{210, 0, 210});
        CHECK_THROWS(data.get(-1, 0));
        CHECK_THROWS(data.get(0, -1));
        CHECK_THROWS(data.get(11, 0));
        CHECK_THROWS(data.get(0, 11));

        CHECK_THROWS(data.get_worldspace(Coordinate{199, 0, 200}));
        CHECK_THROWS(data.get_worldspace(Coordinate{200, 0, 199}));
        CHECK_THROWS(data.get_worldspace(Coordinate{211, 0, 200}));
        CHECK_THROWS(data.get_worldspace(Coordinate{200, 0, 211}));

        Coordinate to_fill{199, 0, 211};
        CHECK_THROWS(data.fill_coord(to_fill));
    }

    SUBCASE("Negative coord") {
        mc.setBlocks(Coordinate{-200, 300, -200}, Coordinate{-210, 319, -210},
                     Blocks::AIR);
        mc.setBlocks(Coordinate{-200, 300, -200}, Coordinate{-210, 300, -210},
                     Blocks::STONE);
        mc.setBlock(Coordinate{-200, 301, -200}, Blocks::STONE);
        mc.setBlock(Coordinate{-210, 301, -210}, Blocks::STONE);
        mc.setBlock(Coordinate{-201, 301, -202}, Blocks::STONE);

        HeightMap data =
            mc.getHeights(Coordinate{-200, 0, -200}, Coordinate{-210, 0, -210});
        CHECK_EQ(data.get_worldspace(Coordinate{-200, 0, -200}), 301);
        CHECK_EQ(data.get_worldspace(Coordinate{-201, 0, -201}), 300);
        CHECK_EQ(data.get_worldspace(Coordinate{-210, 0, -210}), 301);
        CHECK_EQ(data.get_worldspace(Coordinate{-201, 0, -202}), 301);
    }

    // Clean up
    mc.setBlocks(Coordinate{200, 300, 200}, Coordinate{210, 301, 210},
                 Blocks::AIR);
}
// Requires player joined to server, will throw serverside if player is not
// joined
#ifdef PLAYER_TEST

TEST_CASE("Player operations") {
    Coordinate test_loc{110, 110, 110};
    mc.setBlock(test_loc, Blocks::DIRT);

    SUBCASE("Execute command") { mc.doCommand("time set noon"); }

    SUBCASE("Set position") {
        mc.setPlayerPosition(test_loc + Coordinate(0, 1, 0));
    }

    SUBCASE("Get position") {
        mc.setPlayerPosition(Coordinate(0, 0, 0));
        mc.setPlayerPosition(test_loc + Coordinate(0, 1, 0));
        Coordinate player_loc = mc.getPlayerPosition();
        CHECK((player_loc == (test_loc + Coordinate(0, 1, 0))));
    }

    SUBCASE("Check correct flooring") {
        Coordinate negative_loc(-2, 100, -2);
        mc.doCommand("tp -2 100 -2");
        CHECK_EQ(mc.getPlayerPosition(), negative_loc);
    }

    SUBCASE("setPlayerTilePosition and getPlayerTilePosition") {
        mc.setPlayerPosition(Coordinate(0, 0, 0));

        mc.setPlayerTilePosition(test_loc);

        Coordinate result = mc.getPlayerTilePosition();
        Coordinate expected = test_loc;

        CHECK_EQ(result, expected);

        Coordinate p_result = mc.getPlayerPosition();
        Coordinate p_expected = test_loc + Coordinate(0, 1, 0);

        CHECK_EQ(p_result, p_expected);
    }

    // Cleanup
    mc.setBlock(test_loc, Blocks::AIR);
}

#endif
