#pragma once

#include <cmath>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

template <typename T> void split_response(const std::string& str, std::vector<T>& vec) {
  static_assert(std::is_integral_v<T>, "T must be an integral type.");

  std::stringstream ss(str);
  std::string item;

  while (std::getline(ss, item, ',')) {
    try {
      vec.push_back(static_cast<T>(std::floor(std::stod(item))));
    } catch (const std::exception&) {
      throw std::runtime_error("Server call returned malformed response string: " + str);
    }
  }
}
