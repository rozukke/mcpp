#pragma once
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <stdexcept>

class SocketConnection {
public:
    explicit SocketConnection(std::string_view address_str = "localhost", int port = 4711);
    void send(std::string_view data_str) const;
    std::string recv();

private:
    const int INVALID_SOCKET = -1;
    struct sockaddr_in addr{};
    std::string response;
    int sockfd;
    void conn();
};
