#ifndef CYCLEDETECT_HEADER_H
#define CYCLEDETECT_HEADER_H

#include <vector>

#include <iostream>
#include "HeaderInclude.h"
#include "MiscUtils.h"

class Header {

  unsigned LineCounter = 0;

  StringCache::ID Path;

  std::vector<HeaderInclude> IncludedHeaders;

public:
  Header(const std::string& P) : Path(StringCache::get()[normalizePath(P)]) {
  }

  Header(const std::string& P, const IncludePaths& Includes);

  void parseLine(const std::string& Line, const IncludePaths& Includes);

  StringCache::ID getPath() const {
    return Path;
  }

  std::vector<HeaderInclude>& getIncludedHeaders() {
    return IncludedHeaders;
  }

  const std::vector<HeaderInclude>& getIncludedHeaders() const {
    return IncludedHeaders;
  }

};


#endif //CYCLEDETECT_HEADER_H
