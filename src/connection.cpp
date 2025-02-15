#include "connection.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <sstream>
#include <stdexcept>

namespace mcpp {
SocketConnection::SocketConnection(const std::string& address_str, uint16_t port) {
  std::string ip_addr = resolve_hostname(address_str);

  // Using std libs only to avoid dependency on socket lib
  _socket_handle = socket(AF_INET, SOCK_STREAM, 0);
  if (_socket_handle == -1) {
    throw std::runtime_error("Failed to create socket.");
  }

  sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);

  if (inet_pton(AF_INET, ip_addr.c_str(), &(server_addr.sin_addr)) <= 0) {
    throw std::runtime_error("Invalid address.");
  }

  if (connect(_socket_handle, reinterpret_cast<struct sockaddr*>(&server_addr),
              sizeof(server_addr)) < 0) {
    throw std::runtime_error("Failed to connect to the server. Check if the server is running.");
  }
}

std::string SocketConnection::resolve_hostname(const std::string& hostname) {
  struct addrinfo hints {};
  struct addrinfo* result;

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  if (getaddrinfo(hostname.c_str(), nullptr, &hints, &result) != 0) {
    throw std::runtime_error("Failed to resolve hostname.");
  }

  auto* address = reinterpret_cast<struct sockaddr_in*>(result->ai_addr);
  char ip_addr[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(address->sin_addr), ip_addr, INET_ADDRSTRLEN);

  std::string ip_string(ip_addr);
  freeaddrinfo(result);

  return ip_string;
}

void SocketConnection::send(const std::string& data_string) {
  _last_sent = data_string;
  ssize_t result = write(_socket_handle, data_string.c_str(), data_string.length());
  if (result < 0) {
    throw std::runtime_error("Failed to send data.");
  }
}

std::string SocketConnection::recv() const {
  std::stringstream response_stream;
  char buffer[BUFFER_SIZE];

  ssize_t bytes_read;
  do {
    bytes_read = read(_socket_handle, buffer, sizeof(buffer));
    if (bytes_read < 0) {
      throw std::runtime_error("Failed to receive data.");
    }

    response_stream.write(buffer, bytes_read);
  } while (buffer[bytes_read - 1] != '\n');

  std::string response = response_stream.str();

  // Remove trailing \n
  if (!response.empty() && response[response.length() - 1] == '\n') {
    response.pop_back();
  }

  if (response == FAIL_RESPONSE) {
    std::string error_msg = "Server failed to execute command: ";
    error_msg += _last_sent;
    throw std::runtime_error(error_msg);
  }
  return response;
}
} // namespace mcpp
