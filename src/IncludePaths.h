#ifndef CYCLEDETECT_INCLUDEPATHS_H
#define CYCLEDETECT_INCLUDEPATHS_H

#include <boost/filesystem.hpp>

class IncludePaths {
  std::vector<std::string> Paths;

public:
  IncludePaths(bool AddCurrentDir = true) {
    if (AddCurrentDir)
      addPath(".");
  }

  void addPath(const std::string& Path) {
    Paths.push_back(Path);
  }

  std::string findFile(const std::string& PathStr) const;
};


#endif //CYCLEDETECT_INCLUDEPATHS_H
