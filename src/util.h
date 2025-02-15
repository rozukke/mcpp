#pragma once

#include <cmath>
#include <sstream>
#include <string>
#include <vector>

template <typename T> void split_response(const std::string& str, std::vector<T>& vec) {
  std::stringstream ss(str);
  std::string item;
  while (std::getline(ss, item, ',')) {
    if constexpr (std::is_floating_point_v<T>) {
      vec.push_back(static_cast<T>(std::stod(item)));
    } else if constexpr (std::is_integral_v<T>) {
      double item_double = std::stod(item);
      vec.push_back(static_cast<T>(std::floor(item_double)));
    } else {
      static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type.");
    }
  }
}
