#include "Header.h"

#include <fstream>

void Header::parseLine(const std::string &Line, const IncludePaths &Includes) {
  LineCounter++;
  HeaderInclude include;
  if (include.parse(Line, Includes)) {
    include.setLine(LineCounter);
    IncludedHeaders.push_back(include);
  }
}

Header::Header(const std::string &P, const IncludePaths &Includes) : Path(StringCache::get()[normalizePath(P)]) {

  std::ifstream File(P);
  std::string Line;
  while (std::getline(File, Line)) {
    parseLine(Line, Includes);
  }
}
