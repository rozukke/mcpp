#include "doctest.h"
#include "../src/connection.h"

using std::string;

/*
 * All tests require a running instance of Spigot server with the ELCI plugin in order to run successfully. This
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
    }

    //TODO do additional tests on recv
    SUBCASE("Test receive") {
        tcp_conn.send("world.setBlock(0,0,0,0)\n");
        tcp_conn.send("world.getBlock(0,0,0)\n");
        string return_str = tcp_conn.recv();
        CHECK((return_str == string("0\n")));
    }

    SUBCASE("Repeated receive") {
        tcp_conn.send("world.getBlock(0,0,0)\n");
        string return_str = tcp_conn.recv();
        CHECK((return_str == string("0\n")));
        tcp_conn.send("world.getBlock(0,0,0)\n");
        return_str = tcp_conn.recv();
        CHECK((return_str == string("0\n")));
    }

    //TODO destructor
    SUBCASE("Destructor") {
//        delete tcp_conn;
    }
}
