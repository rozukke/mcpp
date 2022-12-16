#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdexcept>
#include "connection.h"

#define MAX_PACKET_SIZE 4096

using std::string, std::string_view;

SocketConnection::SocketConnection(string_view address_str, int port) {
    //init socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        throw std::runtime_error("Error creating socket");
    }


    //parse arg addresses
    if (address_str == "localhost") {
        address_str = "127.0.0.1";
    }
    inet_aton(address_str.data(), &addr.sin_addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    conn();
}

void SocketConnection::conn() {
    const int result = ::connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));
    if (result == -1) {
        throw std::runtime_error("Connection Failed");
    }
}

void SocketConnection::send(string_view data_str) const {
    ::send(sockfd, data_str.data(), data_str.length(), 0);
}

string SocketConnection::recv() const {
    char msg[MAX_PACKET_SIZE];
    const size_t bytes_received = ::recv(sockfd, msg, MAX_PACKET_SIZE, 0);
//    std::cout << (string)msg << "of size" << bytes_received <<std::endl;
    return (string) msg;
}