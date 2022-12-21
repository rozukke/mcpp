#pragma once

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <stdexcept>
#include <initializer_list>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

namespace mcpp {

    class SocketConnection {
    private:
        boost::asio::ip::tcp::socket *socket;
        const std::string FailedCommandResponse = "Fail";

    public:
        SocketConnection(std::string_view address_str = "localhost", uint16_t port = 4711);

        void send(std::string_view data_str) const;

        [[nodiscard]] std::string recv() const;

        template<typename... Types>
        void sendCommand(const std::string &prefix, Types ...args) {
            std::stringstream ss;

            ss << prefix << "(";

            // iterate over args pack
            ((ss << args << ','), ...);
            // Remove trailing comma
            ss.seekp(-1, std::ios_base::end);

            ss << ")\n";

            send(ss.str());
        }

        template<typename T, typename... Types>
        std::string sendReceiveCommand(T prefix, Types ...args) {
            sendCommand(prefix, args...);
            auto result = recv();
            return result;
        }

        SocketConnection &operator=(const SocketConnection &other);
    };
}