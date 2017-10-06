#ifndef CYCLEDETECT_HEADER_H
#define CYCLEDETECT_HEADER_H

#include <vector>

#include <iostream>
#include "HeaderInclude.h"
#include "MiscUtils.h"

class Header {

  StringCache::ID Path;
  StringCache::ID AbsPath;

  std::vector<HeaderInclude> IncludedHeaders;

public:
  Header(const std::string& P, const std::string& AbsPath)
      : Path(StringCache::get()[normalizePath(P)]), AbsPath(StringCache::get()[normalizePath(AbsPath)]) {
  }

  Header(const std::string& P, const std::string& AbsPath, const IncludePaths& Includes);

  void parseLine(const std::string& Line, const IncludePaths& Includes, unsigned LineNo);

  StringCache::ID getPath() const {
    return Path;
  }

  StringCache::ID getAbsPath() const {
    return AbsPath;
  }

  std::vector<HeaderInclude>& getIncludedHeaders() {
    return IncludedHeaders;
  }

  const std::vector<HeaderInclude>& getIncludedHeaders() const {
    return IncludedHeaders;
  }

  bool operator<(const Header& Other) const {
    return AbsPath < Other.AbsPath;
  }
};


#endif //CYCLEDETECT_HEADER_H
