#include "Header.h"

#include <fstream>

void Header::parseLine(const std::string &Line, const IncludePaths &Includes, unsigned LineNo) {
  HeaderInclude include;
  if (include.parse(Line, Includes)) {
    include.setLine(LineNo);
    IncludedHeaders.push_back(include);
  }
}

Header::Header(const std::string &P, const std::string &AbsPath, const IncludePaths &Includes)
    : Path(StringCache::get()[normalizePath(P)]), AbsPath(StringCache::get()[normalizePath(AbsPath)]) {

  std::ifstream File(P);
  std::string Line;
  unsigned LineCounter = 0;
  while (std::getline(File, Line)) {
    LineCounter++;
    parseLine(Line, Includes, LineCounter);
  }
}
