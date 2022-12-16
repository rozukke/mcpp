#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../src/util.h"

/*
 * Used to test code that is not connection dependent such as the implementation of Coordinate and various other
 * utility functions as well as Block functionality.
 */

TEST_CASE("Test Coordinate class"){
    SUBCASE("Test init") {
        Coordinate testCoord;
        CHECK((testCoord.x == 0));
        CHECK((testCoord.y == 0));
        CHECK((testCoord.z == 0));
    }

    SUBCASE("Test double init") {
        Coordinate testCoord(1.5, 2.5, 3.5);
        Coordinate testCoordFloat(1.5f, 2.5f, 3.5f);
        Coordinate testCoordRHS(1, 2, 3);

        CHECK((testCoord == testCoordRHS));
        CHECK((testCoord == testCoordFloat));
    }

    SUBCASE("Test equals") {
        Coordinate testCoord(3, 2, 1);
        Coordinate testCoordRHS(3, 2, 1);

        CHECK((testCoord == testCoordRHS));
    }

    SUBCASE("Test add") {
        Coordinate testCoord(3, 2, 1);
        Coordinate testCoordRHS(1, 2, 3);
        Coordinate result(4, 4, 4);

        CHECK(((testCoord + testCoordRHS) == result));
    }

    SUBCASE("Test subtract") {
        Coordinate testCoord(1, 2, 3);
        Coordinate testCoordRHS(0, 2, 4);

        Coordinate result(1, 0, -1);

        CHECK(((testCoord - testCoordRHS) == result));
    }

    SUBCASE("Test clone") {
        Coordinate testCoord(1, 2, 3);
        Coordinate testCoordClone = testCoord.clone();

        CHECK((testCoord == testCoordClone));
        CHECK((&testCoord != &testCoordClone));
    }
}
