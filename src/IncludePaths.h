#ifndef CYCLEDETECT_INCLUDEPATHS_H
#define CYCLEDETECT_INCLUDEPATHS_H

#include <boost/filesystem.hpp>

class IncludePaths {
  std::vector<std::string> Paths;

public:
  IncludePaths() {
  }

  void addPath(std::string Path);

  void addPaths(const std::vector<std::string>& NewPaths) {
    for (const auto &Path : NewPaths)
      addPath(Path);
  }

  void addPaths(const IncludePaths& ToAdd) {
    for (const auto &Path : ToAdd.Paths)
      addPath(Path);
  }

  std::string findFile(const std::string& PathStr) const;
};


#endif //CYCLEDETECT_INCLUDEPATHS_H
