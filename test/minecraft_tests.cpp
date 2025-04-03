#include "../include/mcpp/mcpp.h"
#include "../src/connection.h"
#include "doctest.h"

// NOLINTBEGIN

using namespace std::string_literals;
using namespace mcpp;

// You may need to set the address to your $(hostname).local if running on WSL2.
SocketConnection tcp_conn("localhost", mcpp::MCPP_PORT);
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
    tcp_conn.send("world.set_block(100,100,100,30)\n");
    tcp_conn.send("world.get_block(100,100,100)\n");
    std::string return_str = tcp_conn.recv();
    CHECK_EQ(return_str, "30");
    tcp_conn.send("world.set_block(100,100,100,0)\n");
  }

  SUBCASE("Repeated receive") {
    tcp_conn.send("world.set_block(100,100,100,29)\n");
    tcp_conn.send("world.get_block(100,100,100)\n");
    std::string return_str = tcp_conn.recv();
    CHECK_EQ(return_str, "29");
    tcp_conn.send("world.set_block(100,100,100,0)\n");
  }

  SUBCASE("Send command") { tcp_conn.send_command("chat.post", "test message"); }

  SUBCASE("Send receive command") {
    tcp_conn.send_command("world.set_block", 100, 100, 100, 26);
    auto result = tcp_conn.send_receive_command("world.get_block", 100, 100, 100);
    CHECK_EQ(result, "26");

    tcp_conn.send_command("world.set_block", 100, 100, 100, 25);
    result = tcp_conn.send_receive_command("world.get_block", 100, 100, 100);
    CHECK_EQ(result, "25");
    tcp_conn.send_command("world.set_block", 100, 100, 100, 0);
  }

  // TODO: This is no longer the same size for whatever reason. Please fix or
  // figure out
  /*
   * Validates non-existence of a rare bug where specific response sizes
   * would hang execution.
   */
  //
  // SUBCASE("Test receive when response size is divisible by buffer size") {
  //     // Assuming buffer size is 1024 bytes
  //     int expected_size = 4096;
  //
  //     int x1 = 0, y1 = 0, z1 = 0;
  //     int x2 = 31, y2 = 100, z2 = 31;
  //
  //     tcp_conn.sendCommand("world.set_blocks", x1, y1, z1, x2, y2, z2,
  //                          Blocks::DIRT.id, Blocks::DIRT.mod);
  //     std::string result =
  //         tcp_conn.sendReceiveCommand("world.get_heights", x1, z1, x2, z2);
  //     int real_size = result.size();
  //
  //     // -1 because newline is removed
  //     CHECK_EQ(real_size, expected_size - 1);
  //
  //     // Cleanup
  //     tcp_conn.sendCommand("world.set_blocks", x1, y1, z1, x2, y2, z2,
  //                          Blocks::AIR.id, Blocks::AIR.mod);
  // }

  SUBCASE("Check fail condition") {
    CHECK_THROWS(tcp_conn.send_receive_command("failCommand", ""));
  }
}

TEST_CASE("Test the main mcpp class") {
  Coordinate test_loc(100, 100, 100);

  SUBCASE("post_to_chat") { mc.post_to_chat("test string"); }

  SUBCASE("set_block") { mc.set_block(test_loc, BlockType(50)); }

  SUBCASE("get_block") {
    mc.set_block(test_loc, BlockType(34));
    CHECK_EQ(mc.get_block(test_loc), BlockType(34));
  }

  // Using values from the Blocks struct in block.h beyond this point
  SUBCASE("get_block with mod") {
    mc.set_block(test_loc, BlockType(5, 5));
    CHECK_EQ(mc.get_block(test_loc), BlockType(5, 5));

    mc.set_block(test_loc, Blocks::LIGHT_BLUE_CONCRETE);
    CHECK_EQ(mc.get_block(test_loc), Blocks::LIGHT_BLUE_CONCRETE);
  }

  SUBCASE("get_height") {
    Coordinate heightTestLoc(300, 200, 300);
    mc.set_block(heightTestLoc, Blocks::DIRT);
    auto height = mc.get_height(heightTestLoc.x, heightTestLoc.z);
    CHECK_EQ(height, heightTestLoc.y);

    // Clean up
    mc.set_block(heightTestLoc, Blocks::AIR);
  }

  SUBCASE("set_blocks") {
    Coordinate loc1{100, 100, 100};
    Coordinate loc2{110, 110, 110};
    mc.set_blocks(loc1, loc2, Blocks::STONE);
  }
}

TEST_CASE("get_blocks and Chunk operations") {

  // Setup
  Coordinate test_loc(100, 100, 100);
  Coordinate loc1{100, 100, 100};
  Coordinate loc2{110, 111, 112};

  // Reset blocks that existed before
  mc.set_blocks(loc1, loc2, Blocks::AIR);
  mc.set_blocks(loc1, loc2, Blocks::BRICKS);
  mc.set_block(loc1, Blocks::GOLD_BLOCK);
  mc.set_block(loc2, Blocks::DIAMOND_BLOCK);
  mc.set_block(loc1 + Coordinate{1, 2, 3}, Blocks::IRON_BLOCK);
  Chunk res = mc.get_blocks(loc1, loc2);
  const Chunk res_const = mc.get_blocks(loc1, loc2);

  SUBCASE("getters") {
    Chunk data = mc.get_blocks(loc1, loc2);

    CHECK_EQ(data.base_pt(), loc1);
    CHECK_EQ(data.x_len(), 11);
    CHECK_EQ(data.y_len(), 12);
    CHECK_EQ(data.z_len(), 13);

    data = mc.get_blocks(loc2, loc1);

    CHECK_EQ(data.base_pt(), loc1);
    CHECK_EQ(data.x_len(), 11);
    CHECK_EQ(data.y_len(), 12);
    CHECK_EQ(data.z_len(), 13);
  }

  SUBCASE("Block accessing returns correct block using get()") {
    CHECK_EQ(res.get(0, 0, 0), Blocks::GOLD_BLOCK);
    CHECK_EQ(res.get(1, 1, 1), Blocks::BRICKS);
    CHECK_EQ(res.get(1, 2, 3), Blocks::IRON_BLOCK);
    CHECK_EQ(res.get(10, 11, 12), Blocks::DIAMOND_BLOCK);
  }

  SUBCASE("Block accessing returns correct block using get_worldspace()") {
    CHECK_EQ(res.get_worldspace(loc1), Blocks::GOLD_BLOCK);
    CHECK_EQ(res.get_worldspace(loc1 + Coordinate{1, 1, 1}), Blocks::BRICKS);
    CHECK_EQ(res.get_worldspace(loc1 + Coordinate{1, 2, 3}), Blocks::IRON_BLOCK);
    CHECK_EQ(res.get_worldspace(loc2), Blocks::DIAMOND_BLOCK);
  }

  SUBCASE("Access out of bounds correctly throws") {
    CHECK_THROWS(res.get(11, 0, 0));
    CHECK_THROWS(res.get(0, 12, 0));
    CHECK_THROWS(res.get(0, 0, 13));
    CHECK_THROWS(res.get(-1, 0, 0));
    CHECK_THROWS(res.get(0, -1, 0));
    CHECK_THROWS(res.get(0, 0, -1));
    CHECK_THROWS(res.get_worldspace(loc1 + Coordinate{-1, -1, -1}));
    CHECK_THROWS(res.get_worldspace(loc1 + Coordinate{11, 12, 13}));
  }

  SUBCASE("Iterator") {
    std::vector<BlockType> blocks;
    for (int i = 0; i < res.y_len(); i++) {
      for (int j = 0; j < res.x_len(); j++) {
        for (int z = 0; z < res.z_len(); z++) {
          blocks.push_back(res.get(j, i, z));
        }
      }
    }

    std::vector<BlockType> expected_blocks;
    for (BlockType block : res) {
      expected_blocks.push_back(block);
    }
    CHECK_EQ(blocks, expected_blocks);
  }

  SUBCASE("Const iterator") {
    std::vector<BlockType> blocks;
    for (int i = 0; i < res_const.y_len(); i++) {
      for (int j = 0; j < res_const.x_len(); j++) {
        for (int z = 0; z < res_const.z_len(); z++) {
          blocks.push_back(res_const.get(j, i, z));
        }
      }
    }

    std::vector<BlockType> expected_blocks;
    for (BlockType block : res_const) {
      expected_blocks.push_back(block);
    }
    CHECK_EQ(blocks, expected_blocks);
  }

  SUBCASE("Constructors & assignment") {
    // Copy assignment
    mc.set_block({10, 10, 10}, Blocks::BLUE_CONCRETE);
    auto chunk = mc.get_blocks({10, 10, 10}, {20, 20, 20});
    Chunk chunk_copy = chunk; // Contains BLUE
    CHECK_EQ(chunk.get(0, 0, 0), chunk_copy.get(0, 0, 0));

    // Reassignment
    mc.set_block({10, 10, 10}, Blocks::RED_CONCRETE);
    chunk = mc.get_blocks({10, 10, 10}, {20, 20, 20}); // Now contains RED
    CHECK_NE(chunk.get(0, 0, 0), chunk_copy.get(0, 0, 0));

    // Move assignment
    chunk = std::move(chunk_copy); // Now contains BLUE
    CHECK_EQ(chunk.get(0, 0, 0), Blocks::BLUE_CONCRETE);

    // Copy constructor
    auto chunk_copy2 = Chunk(chunk); // Contains BLUE
    mc.set_block({10, 10, 10}, Blocks::WHITE_CONCRETE);
    chunk = mc.get_blocks({10, 10, 10}, {20, 20, 20}); // Now contains WHITE
    CHECK_NE(chunk_copy2.get(0, 0, 0), Blocks::WHITE_CONCRETE);

    // Move constructor
    chunk = Chunk(std::move(chunk_copy2)); // Now contains BLUE
    CHECK_EQ(chunk.get(0, 0, 0), Blocks::BLUE_CONCRETE);
  }

  mc.set_block(test_loc, BlockType(0));
}

TEST_CASE("Test blocks struct") {
  Coordinate testLoc;
  mc.set_block(testLoc, Blocks::AIR);
  CHECK_EQ(mc.get_block(testLoc), Blocks::AIR);
  mc.set_block(testLoc, Blocks::STONE);
  CHECK_EQ(mc.get_block(testLoc), Blocks::STONE);
}

TEST_CASE("HeightMap functionality") {
  // 319 is the build limit in 1.19
  mc.set_blocks(Coordinate{200, 300, 200}, Coordinate{210, 319, 210}, Blocks::AIR);
  mc.set_blocks(Coordinate{200, 300, 200}, Coordinate{210, 300, 210}, Blocks::STONE);
  mc.set_block(Coordinate{200, 301, 200}, Blocks::STONE);
  mc.set_block(Coordinate{210, 301, 210}, Blocks::STONE);
  mc.set_block(Coordinate{201, 301, 202}, Blocks::STONE);

  SUBCASE("getters") {
    HeightMap data = mc.get_heights(Coordinate{200, 0, 200}, Coordinate{210, 0, 210});

    CHECK_EQ(data.base_pt(), Coordinate{200, 0, 200});
    CHECK_EQ(data.x_len(), 11);
    CHECK_EQ(data.z_len(), 11);

    data = mc.get_heights(Coordinate{210, 300, 210}, Coordinate{200, 310, 200});

    CHECK_EQ(data.base_pt(), Coordinate{200, 0, 200});
    CHECK_EQ(data.x_len(), 11);
    CHECK_EQ(data.z_len(), 11);
  }

  SUBCASE("get") {
    HeightMap data = mc.get_heights(Coordinate{200, 0, 200}, Coordinate{210, 0, 210});
    CHECK_EQ(data.get(0, 0), 301);
    CHECK_EQ(data.get(1, 1), 300);
    CHECK_EQ(data.get(10, 10), 301);
    CHECK_EQ(data.get(1, 2), 301);
  }

  SUBCASE("get_worldspace") {
    HeightMap data = mc.get_heights(Coordinate{200, 0, 200}, Coordinate{210, 0, 210});
    CHECK_EQ(data.get_worldspace(Coordinate{200, 0, 200}), 301);
    CHECK_EQ(data.get_worldspace(Coordinate{201, 0, 201}), 300);
    CHECK_EQ(data.get_worldspace(Coordinate{210, 0, 210}), 301);
    CHECK_EQ(data.get_worldspace(Coordinate{201, 0, 202}), 301);
  }

  SUBCASE("fill_coord") {
    HeightMap data = mc.get_heights(Coordinate{200, 0, 200}, Coordinate{210, 0, 210});

    Coordinate to_fill{200, 0, 200};
    data.fill_coord(to_fill);
    CHECK_EQ(to_fill.y, 301);
  }

  SUBCASE("Bounds checking") {
    HeightMap data = mc.get_heights(Coordinate{200, 0, 200}, Coordinate{210, 0, 210});
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
    mc.set_blocks(Coordinate{-200, 300, -200}, Coordinate{-210, 319, -210}, Blocks::AIR);
    mc.set_blocks(Coordinate{-200, 300, -200}, Coordinate{-210, 300, -210}, Blocks::STONE);
    mc.set_block(Coordinate{-200, 301, -200}, Blocks::STONE);
    mc.set_block(Coordinate{-210, 301, -210}, Blocks::STONE);
    mc.set_block(Coordinate{-201, 301, -202}, Blocks::STONE);

    HeightMap data = mc.get_heights(Coordinate{-200, 0, -200}, Coordinate{-210, 0, -210});
    CHECK_EQ(data.get_worldspace(Coordinate{-200, 0, -200}), 301);
    CHECK_EQ(data.get_worldspace(Coordinate{-201, 0, -201}), 300);
    CHECK_EQ(data.get_worldspace(Coordinate{-210, 0, -210}), 301);
    CHECK_EQ(data.get_worldspace(Coordinate{-201, 0, -202}), 301);
  }

  SUBCASE("Iterator") {
    mc.set_blocks(Coordinate{-200, 300, -200}, Coordinate{-210, 319, -210}, Blocks::AIR);
    mc.set_blocks(Coordinate{-200, 300, -200}, Coordinate{-210, 300, -210}, Blocks::STONE);
    mc.set_block(Coordinate{-200, 301, -200}, Blocks::STONE);
    mc.set_block(Coordinate{-210, 301, -210}, Blocks::STONE);
    mc.set_block(Coordinate{-201, 301, -202}, Blocks::STONE);

    HeightMap data = mc.get_heights(Coordinate{-200, 0, -200}, Coordinate{-210, 0, -210});

    std::vector<int> expected_heights;
    for (int i = 0; i < data.x_len(); i++) {
      for (int j = 0; j < data.z_len(); j++) {
        expected_heights.push_back(data.get(i, j));
      }
    }

    std::vector<int> heights;
    for (int height : data) {
      heights.push_back(height);
    }
    CHECK_EQ(heights, expected_heights);
  }

  SUBCASE("Const iterator") {
    mc.set_blocks(Coordinate{-200, 300, -200}, Coordinate{-210, 319, -210}, Blocks::AIR);
    mc.set_blocks(Coordinate{-200, 300, -200}, Coordinate{-210, 300, -210}, Blocks::STONE);
    mc.set_block(Coordinate{-200, 301, -200}, Blocks::STONE);
    mc.set_block(Coordinate{-210, 301, -210}, Blocks::STONE);
    mc.set_block(Coordinate{-201, 301, -202}, Blocks::STONE);

    const HeightMap data = mc.get_heights(Coordinate{-200, 0, -200}, Coordinate{-210, 0, -210});

    std::vector<int> expected_heights;
    for (int i = 0; i < data.x_len(); i++) {
      for (int j = 0; j < data.z_len(); j++) {
        expected_heights.push_back(data.get(i, j));
      }
    }

    std::vector<int> heights;
    for (int height : data) {
      heights.push_back(height);
    }
    CHECK_EQ(heights, expected_heights);
  }

  SUBCASE("Constructors & assignment") {
    // Copy assignment
    mc.set_blocks({10, 310, 10}, {20, 320, 20}, Blocks::AIR);
    mc.set_blocks({10, 310, 10}, {20, 310, 20}, Blocks::STONE);
    auto map = mc.get_heights({10, 10, 10}, {20, 20, 20});
    HeightMap map_copy = map; // Contains 310
    CHECK_EQ(map.get(0, 0), map_copy.get(0, 0));
    CHECK_EQ(map.get(0, 0), 310);

    // Reassignment
    mc.set_block({10, 311, 10}, Blocks::STONE);
    map = mc.get_heights({10, 10, 10}, {20, 20, 20}); // Now contains 311
    CHECK_NE(map.get(0, 0), map_copy.get(0, 0));
    CHECK_EQ(map.get(0, 0), 311);

    // Move assignment
    map = std::move(map_copy); // Now contains 310
    CHECK_EQ(map.get(0, 0), 310);

    // Copy constructor
    auto map_copy2 = HeightMap(map); // Contains 310
    mc.set_block({10, 312, 10}, Blocks::STONE);
    map = mc.get_heights({10, 10, 10}, {20, 20, 20}); // Now contains 312
    CHECK_NE(map_copy2.get(0, 0), 312);
    CHECK_EQ(map.get(0, 0), 312);

    // Move constructor
    map = HeightMap(std::move(map_copy2)); // Now contains 310
    CHECK_EQ(map.get(0, 0), 310);

    mc.set_blocks({10, 310, 10}, {20, 320, 20}, Blocks::AIR);
  }

  // Clean up
  mc.set_blocks(Coordinate{200, 300, 200}, Coordinate{210, 301, 210}, Blocks::AIR);
}

// Requires player joined to server, will throw serverside if player is not
// joined
#ifdef PLAYER_TEST

TEST_CASE("Player operations") {
  Coordinate test_loc{110, 110, 110};
  mc.set_block(test_loc, Blocks::DIRT);

  SUBCASE("Execute command") { mc.do_command("time set noon"); }

  SUBCASE("Set position") { mc.set_player_position(test_loc + Coordinate(0, 1, 0)); }

  SUBCASE("Get position") {
    mc.set_player_position(Coordinate(0, 0, 0));
    mc.set_player_position(test_loc + Coordinate(0, 1, 0));
    Coordinate player_loc = mc.get_player_position();
    CHECK((player_loc == (test_loc + Coordinate(0, 1, 0))));
  }

  SUBCASE("Check correct flooring") {
    Coordinate negative_loc(-2, 100, -2);
    mc.do_command("tp -2 100 -2");
    CHECK_EQ(mc.get_player_position(), negative_loc);
  }

  SUBCASE("set_player_tile_position and get_player_tile_position") {
    mc.set_player_position(Coordinate(0, 0, 0));

    mc.set_player_tile_position(test_loc);

    Coordinate result = mc.get_player_tile_position();
    Coordinate expected = test_loc;

    CHECK_EQ(result, expected);

    Coordinate p_result = mc.get_player_position();
    Coordinate p_expected = test_loc + Coordinate(0, 1, 0);

    CHECK_EQ(p_result, p_expected);
  }

  // Cleanup
  mc.set_block(test_loc, Blocks::AIR);
}

#endif

// NOLINTEND
