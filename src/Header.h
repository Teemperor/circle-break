#ifndef CYCLEDETECT_HEADER_H
#define CYCLEDETECT_HEADER_H

#include <vector>


#include "HeaderInclude.h"

class Header {

  unsigned LineCounter = 0;

  std::string Path;

  std::vector<HeaderInclude> IncludedHeaders;

public:
  Header(const std::string& Path) : Path(Path) {
  }

  Header(const std::string& Path, const IncludePaths& Includes) : Path(Path) {
    std::ifstream File(Path);
    std::string Line;
    while (std::getline(File, Line)) {
      parseLine(Line, Includes);
    }
  }

  void parseLine(const std::string& Line, const IncludePaths& Includes) {
    LineCounter++;
    HeaderInclude include;
    if (include.parse(Line, Includes)) {
      include.setLine(LineCounter);
      IncludedHeaders.push_back(include);
    }
  }

  const std::string& getPath() const {
    return Path;
  }

  const std::vector<HeaderInclude>& getIncludedHeaders() const {
    return IncludedHeaders;
  }

};


#endif //CYCLEDETECT_HEADER_H
