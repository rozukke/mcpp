#include <string>
#include "connection.h"
#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;
using namespace boost;
using std::string, std::string_view;


namespace mcpp {
    SocketConnection::SocketConnection(string_view address_str, uint16_t port) {

        asio::io_context io_context;
        this->socket = new tcp::socket(io_context);
        tcp::resolver resolver(io_context);
        asio::connect(*socket, resolver.resolve(address_str, std::to_string(port)));
    }

    SocketConnection &SocketConnection::operator=(const SocketConnection &other) {
        return *this;
    }

    void SocketConnection::send(string_view data_str) const {
        socket->write_some(asio::buffer(data_str));
//        asio::write(*socket, asio::buffer(data_str));
    }

    string SocketConnection::recv() const {
//        std::cout << "Receiving message: ";
        std::string response;
        asio::read_until(*socket, asio::dynamic_buffer(response), '\n');
//        std::cout << response.data() << std::endl;

        //remove trailing \n
        response.pop_back();

        return response;
    }

}