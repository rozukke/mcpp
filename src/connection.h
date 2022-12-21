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
        void sendCommand(const std::string& prefix, Types ...args) {
            std::string stringBuild;

            stringBuild.append(prefix);
            stringBuild.append("(");

            // iterate over args pack
            ((stringBuild.append(handleArg(args))), ...);

            stringBuild.pop_back();
            stringBuild.append(")\n");

//            std::cout << stringBuild << std::endl;
            send(stringBuild);
        }

        template<typename T, typename... Types>
        std::string sendReceiveCommand(T prefix, Types ...args) {
            sendCommand(prefix, args...);
            auto result = recv();
            return result;
        }

        SocketConnection &operator=(const SocketConnection &other);

    private:

        // Used within sendCommand to handle different types provided to it
        template<typename T>
        std::string handleArg(T arg) {
            std::stringstream ss;
            ss << arg << ",";
            return ss.str();
        }

    };
}