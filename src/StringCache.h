#ifndef CYCLEDETECT_STRINGCACHE_H
#define CYCLEDETECT_STRINGCACHE_H

#include <unordered_map>
#include <vector>
#include <cassert>

class StringCache {
public:
  class ID {
    size_t InternalID;
  public:
    ID() { InternalID = 0; }
    ID(size_t InternalID) : InternalID(InternalID) {}
    ID(const ID& Other) {
      InternalID = Other.InternalID;
    }
    ID(const std::string& str) {
      InternalID = StringCache::get()[str].InternalID;
    }
    ID& operator=(const ID& Other) {
      InternalID = Other.InternalID;
      return *this;
    }

    bool valid() const {
      return InternalID != 0;
    }

    const std::string& str() const;

    bool operator==(const ID& Other) const {
      return InternalID == Other.InternalID;
    }

    bool operator!=(const ID& Other) const {
      return InternalID != Other.InternalID;
    }

    bool operator==(const std::string& Other) const {
      return *this == StringCache::get()[Other];
    }

    bool operator!=(const std::string& Other) const {
      return *this != StringCache::get()[Other];
    }

    friend class StringCache;
  };

private:
  std::unordered_map<std::string, size_t> Cache;
  std::vector<std::string> Data = {"INVALID"};


public:

  static StringCache &get() {
    static StringCache Instance;
    return Instance;
  }

  const std::string& operator[](const ID& Lookup) const {
    assert(Lookup.InternalID != 0);
    assert(Lookup.InternalID < Data.size());
    return Data[Lookup.InternalID];
  }

  ID operator[](const std::string &str) {
    auto I = Cache.find(str);
    if (I == Cache.end()) {
      size_t NewID = Cache.size() + 1;
      Cache[str] = NewID;
      Data.push_back(str);
      return NewID;
    } else {
      return I->second;
    }
  }

};



#endif //CYCLEDETECT_STRINGCACHE_H
