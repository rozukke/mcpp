#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "../include/mcpp/util.h"
#include "../include/mcpp/block.h"
#include "../include/mcpp/entity.h"

using namespace mcpp;

/*
 * Used to test code that is not connection dependent such as the implementation of Coordinate and various other
 * utility functions as well as Block functionality.
 */

TEST_CASE("Test Coordinate class") {
    SUBCASE("Test init") {
        Coordinate testCoord;
        CHECK_EQ(testCoord.x, 0);
        CHECK_EQ(testCoord.y, 0);
        CHECK_EQ(testCoord.z, 0);
    }

    SUBCASE("Test double init") {
        Coordinate testCoord(1.5, 2.5, 3.5);
        Coordinate testCoordFloat(1.5f, 2.5f, 3.5f);
        Coordinate testCoordRHS(1, 2, 3);

        CHECK_EQ(testCoord, testCoordRHS);
        CHECK_EQ(testCoord, testCoordFloat);
    }

    SUBCASE("Test equals") {
        Coordinate testCoord(3, 2, 1);
        Coordinate testCoordRHS(3, 2, 1);

        CHECK_EQ(testCoord, testCoordRHS);
    }
    
    SUBCASE("Test inequals") {
        Coordinate testCoord(3, 2, 1);
        Coordinate testCoordRHS(2, 2, 1);

        CHECK_NE(testCoord,testCoordRHS);
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

    SUBCASE("Test clone") {
        Coordinate testCoord(1, 2, 3);
        Coordinate testCoordClone = testCoord.clone();

        CHECK_EQ(testCoord, testCoordClone);
        CHECK_NE(&testCoord, &testCoordClone);
    }

    SUBCASE("Test print") {
        Coordinate testCoord(1, 2, 3);
        std::stringstream ss;
        ss << testCoord;
        CHECK_EQ(ss.str(), "(1, 2, 3)");
    }
}

TEST_CASE("Test block class") {
    SUBCASE("Test equality") {
        BlockType testBlock(10, 2);
        BlockType testBlockRHS(10, 2);
        CHECK_EQ(testBlock, testBlockRHS);
    }

    SUBCASE("Test withMod") {
        BlockType testBlock(10);
        BlockType testBlockRHS(10, 2);
        CHECK_EQ(testBlock.withMod(2), testBlockRHS);

    }

    SUBCASE("Test print") {
        BlockType testBlock(2, 3);
        std::stringstream ss;
        ss << testBlock;
        CHECK_EQ(ss.str(), "[2, 3]");
    }
}

//TEST_CASE("Test entity class") {
//    Entity testEntity = Entities::CHICKEN;
//
//    SUBCASE("Equality operator") {
//        Entity testEntityRHS(93);
//        CHECK_EQ(testEntity, testEntityRHS);
//    }
//}
