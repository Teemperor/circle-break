#include "StringCache.h"

const std::string &StringCache::ID::str() const {
  return StringCache::get()[*this];
}