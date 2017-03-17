#ifndef CYCLEDETECT_HEADER_H
#define CYCLEDETECT_HEADER_H

#include <vector>
#include "HeaderInclude.h"

class Header {

  unsigned LineCounter = 0;

  std::vector<HeaderInclude> IncludedHeaders;

public:
  Header() {
  }

  void parseLine(const std::string& Line, const IncludePaths& Includes) {
    LineCounter++;
    HeaderInclude include;
    if (include.parse(Line, Includes)) {
      include.setLine(LineCounter);
      IncludedHeaders.push_back(include);
    }
  }

  const std::vector<HeaderInclude>& getIncludedHeaders() const {
    return IncludedHeaders;
  }

};


#endif //CYCLEDETECT_HEADER_H
