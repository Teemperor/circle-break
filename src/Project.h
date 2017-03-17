

#ifndef CYCLEDETECT_PROJECT_H
#define CYCLEDETECT_PROJECT_H


#include <boost/filesystem.hpp>
#include <vector>

#include "Module.h"

class Project {

  std::vector<Module> Modules;

public:
  Project(const std::string& path) {
    using namespace boost;
    filesystem::recursive_directory_iterator dir(path), end;

    IncludePaths IncPaths(false);
    IncPaths.addPath(path);

    while (dir != end) {
      if (dir->path().filename() == "interface") {
        Module module;
        module.parseDirectory(dir->path().string(), IncPaths);
        Modules.push_back(module);
      }

      ++dir;
    }

    for (auto& Module : Modules) {
      Module.resolveDependencies(Modules);
    }

  }

  std::vector<DependencyPath> getCycles() const {
    std::vector<DependencyPath> Result;
    for (auto& Module : Modules) {
      for (auto& D : Module.getDependsOn()) {
        if (auto P = D->getPathTo(&Module)) {
          P.add(D);
          Result.push_back(P);
        }
      }
    }

    auto end = std::unique(Result.begin(), Result.end(), [](DependencyPath& P1, DependencyPath& P2){
      return P1.similar(P2);
    });
    Result.erase(end, Result.end());

    return Result;
  }

  const std::vector<Module>& getModules() const {
    return Modules;
  }


};


#endif //CYCLEDETECT_PROJECT_H
