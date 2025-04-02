#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../include/mcpp/block.h"
#include "../include/mcpp/coordinate.h"
#include "doctest.h"
#include <random>

// NOLINTBEGIN

using namespace mcpp;

/*
 * Used to test code that is not connection dependent such as the implementation
 * of Coordinate and various other utility functions as well as Block
 * functionality.
 */

TEST_CASE("Test Coordinate class") {
  SUBCASE("Test init") {
    Coordinate test_coord;
    CHECK_EQ(test_coord.x, 0);
    CHECK_EQ(test_coord.y, 0);
    CHECK_EQ(test_coord.z, 0);
  }

  SUBCASE("Test hash no collision") {
    const int seed = 12345;
    std::set<size_t> hashes;
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dis(-3e7, 3e7);
    std::uniform_int_distribution<int> dis2(-64, 256);

    Coordinate hashing;
    for (int i = 0; i < 100; i++) {
      Coordinate testCoord(dis(gen), dis2(gen), dis(gen));
      size_t hash = hashing(testCoord);
      hashes.insert(hash);
    }
    CHECK_EQ(hashes.size(), 100);
  }

  SUBCASE("Test double init") {
    Coordinate test_coord(1.5, 2.5, 3.5);
    Coordinate test_coord_float(1.5F, 2.5F, 3.5F);
    Coordinate test_coord_rhs(1, 2, 3);

    CHECK_EQ(test_coord, test_coord_rhs);
    CHECK_EQ(test_coord, test_coord_float);
  }

  SUBCASE("Test equals") {
    Coordinate testCoord(3, 2, 1);
    Coordinate testCoordRHS(3, 2, 1);

    CHECK_EQ(testCoord, testCoordRHS);
  }

  SUBCASE("Test not equals") {
    Coordinate testCoord(3, 2, 1);
    Coordinate testCoordRHS(2, 2, 1);

    CHECK(testCoord != testCoordRHS);
    CHECK_NE(testCoord, testCoordRHS);
  }

  SUBCASE("Test add") {
    Coordinate testCoord(3, 2, 1);
    Coordinate testCoordRHS(1, 2, 3);
    Coordinate result(4, 4, 4);

    CHECK_EQ((testCoord + testCoordRHS), result);
  }

  SUBCASE("Test subtract") {
    Coordinate testCoord(1, 2, 3);
    Coordinate testCoordRHS(0, 2, 4);

    Coordinate result(1, 0, -1);

    CHECK_EQ((testCoord - testCoordRHS), result);
  }

  SUBCASE("Test print") {
    Coordinate testCoord(1, 2, 3);
    std::stringstream ss;
    ss << testCoord;
    CHECK_EQ(ss.str(), "(1,2,3)");
  }
}

TEST_CASE("Test block class") {
  SUBCASE("Default ctor") {
    BlockType def;
    CHECK_EQ(def.id, 0);
    CHECK_EQ(def.mod, 0);
  }

  SUBCASE("Test equality") {
    BlockType testBlock(10, 2);
    BlockType testBlockRHS(10, 2);
    CHECK_EQ(testBlock, testBlockRHS);
  }

  SUBCASE("Test non equality") {
    BlockType testBlock(10);
    BlockType testBlockRHS(11);
    CHECK(testBlock != testBlockRHS);
    CHECK_NE(testBlock, testBlockRHS);

    BlockType testBlockWithMod(10, 2);
    BlockType testBlockWithModRHS(10, 3);
    CHECK(testBlockWithMod != testBlockWithModRHS);
    CHECK_NE(testBlockWithMod, testBlockWithModRHS);
  }

  SUBCASE("Test withMod") {
    BlockType testBlock(10);
    BlockType testBlockRHS(10, 2);
    CHECK_EQ(testBlock.with_mod(2), testBlockRHS);
  }

  SUBCASE("Test print") {
    BlockType testBlock(2, 3);
    std::stringstream ss;
    ss << testBlock;
    CHECK_EQ(ss.str(), "[2, 3]");
  }
}

// NOLINTEND
