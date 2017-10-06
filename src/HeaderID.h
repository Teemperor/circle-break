
#ifndef CIRCLE_BREAK_HEADERID_H
#define CIRCLE_BREAK_HEADERID_H

#include <cstdint>
#include <limits>

class Module;

class HeaderID {
  friend class Module;
  std::size_t Value = std::numeric_limits<std::size_t>::max();
public:
  HeaderID() = default;
  bool valid() const {
    return Value != std::numeric_limits<std::size_t>::max();
  }
  operator bool() const {
    return valid();
  }
};


#endif //CIRCLE_BREAK_HEADERID_H
