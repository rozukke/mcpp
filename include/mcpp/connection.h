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
class SocketConnection {
  private:
    int socketHandle;
    std::string lastSent;

    static std::string resolveHostname(const std::string& hostname);

  public:
    SocketConnection(const std::string& address_str, uint16_t port);

    void send(const std::string& dataString);

    /**
     * Takes in parameters supporting std::stringstream conversion and a string
     * prefix and transforms them into format "prefix(arg1,arg2,arg3)" e.g.
     * "chat.post(test)" and sends command to the server.
     *
     * @tparam Types
     * @param prefix
     * @param args
     */
    template <typename... Types>
    void sendCommand(const std::string& prefix, const Types&... args) {
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
    std::string sendReceiveCommand(const T& prefix, const Types&... args) {
        sendCommand(prefix, args...);
        auto result = recv();
        return result;
    }

    [[nodiscard]] std::string recv() const;
};
} // namespace mcpp
