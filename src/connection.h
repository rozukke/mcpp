#pragma once

#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>

#define FAIL_RESPONSE "Fail"

/** @file
 * @brief SocketConnection class.
 *
 */
namespace mcpp {

const size_t BUFFER_SIZE = 1024;

class SocketConnection {
private:
  int _socket_handle;
  std::string _last_sent;

  static std::string resolve_hostname(const std::string& hostname);

public:
  SocketConnection(const std::string& address_str, uint16_t port);

  void send(const std::string& data_string);

  /**
   * Takes in parameters supporting std::stringstream conversion and a string
   * prefix and transforms them into format "prefix(arg1,arg2,arg3)\n" e.g.
   * "chat.post(test)\n)" and sends command to the server.
   *
   * @tparam Types
   * @param prefix
   * @param args
   */
  template <typename... Types> void send_command(const std::string& prefix, const Types&... args) {
    std::stringstream ss;

    ss << prefix << "(";

    // Iterate over args pack
    ((ss << args << ','), ...);
    // Remove trailing comma
    ss.seekp(-1, std::ios_base::end);

    ss << ")\n";

    send(ss.str());
  }

  /**
   * Sends via sendCommand() and returns the result from endpoint
   *
   * @tparam T
   * @tparam Types
   * @param prefix
   * @param args
   * @return
   */
  template <typename T, typename... Types>
  std::string send_receive_command(const T& prefix, const Types&... args) {
    send_command(prefix, args...);
    auto result = recv();
    return result;
  }

  [[nodiscard]] std::string recv() const;
};
} // namespace mcpp
