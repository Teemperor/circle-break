#ifndef CYCLEDETECT_MODULE_H
#define CYCLEDETECT_MODULE_H

#include <boost/filesystem.hpp>
#include <iostream>
#include <unordered_set>

#include "IncludePaths.h"
#include "Header.h"
#include "DependencyPath.h"

struct DependencyData {
  unsigned weight = 0;
};

class Module {

  std::vector<Header> Headers;
  StringCache::ID Path;
  std::string Name;

  void updateName();

  std::map<const Module*, DependencyData> DependsOn;

public:
  Module() {}
  Module(const std::string& P) {
    Path = normalizePath(P);
    updateName();
  }

  Module(const std::string& Path, const IncludePaths& GivenIncludePaths) {
    parseDirectory(Path, GivenIncludePaths);
  }

  void parseDirectory(const std::string& DirectoryPath, const IncludePaths& GivenIncludePaths);

  void addDependency(const Module* M) {
    DependsOn[M].weight++;
  }

  bool hasHeader(StringCache::ID HeaderPath) const {
    for (auto& Header : Headers) {
      if (Header.getPath() == HeaderPath)
        return true;
    }
    return false;
  }

  bool hasHeader(const std::string& HeaderPath) const {
    return hasHeader(StringCache::get()[HeaderPath]);
  }

  std::string getShortPath() const;

  StringCache::ID getPath() const {
    return Path;
  }

  const std::string& getName() const {
    return Name;
  }

  void resolveDependencies(const std::vector<Module>& AllModules);

  DependencyPath getPathTo(const Module* Target) const;

  const std::map<const Module*, DependencyData>& getDependencyInformation() const {
    return DependsOn;
  };

  std::vector<const Module*> getDependsOn() const;

  const std::vector<Header>& getHeaders() const {
    return Headers;
  }

};


#endif //CYCLEDETECT_MODULE_H
