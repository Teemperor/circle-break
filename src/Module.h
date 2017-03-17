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

  void updateName() {
    Name = Path.str();
    if (hasEnding(Name, "/interface")) {
      Name = Name.substr(0, Name.size() - std::string("/interface").size());
    }

    Name.erase(std::remove_if(Name.begin(), Name.end(), [](char c){return !std::isalnum(c);}), Name.end());
  }

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

  void parseDirectory(const std::string& DirectoryPath, const IncludePaths& GivenIncludePaths) {
    Path = normalizePath(DirectoryPath);
    updateName();

    IncludePaths UsedIncludePaths = GivenIncludePaths;
    UsedIncludePaths.addPath(DirectoryPath);

    using namespace boost::filesystem;
    directory_iterator files(DirectoryPath), eod;
    while (files != eod) {
      std::string file = files->path().string();
      Header header(file, UsedIncludePaths);
      Headers.push_back(header);
      files++;
    }
  }

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

  StringCache::ID getPath() const {
    return Path;
  }

  const std::string& getName() const {
    return Name;
  }

  void resolveDependencies(const std::vector<Module>& AllModules) {
    for (auto& Header : Headers) {
      for (auto& Include : Header.getIncludedHeaders()) {
        std::string HeaderPath = Include.getFile().str();

        for (auto& Module : AllModules) {
          if (&Module == this)
            continue;

          if (Module.hasHeader(HeaderPath)) {
            auto I = DependsOn.find(&Module);
            DependsOn[&Module].weight += 1;
          }
        }

      }
    }
  }

  DependencyPath getPathTo(const Module* Target) const {
    std::unordered_set<const Module*> CrossedModules;
    std::vector<DependencyPath> DependencyPaths = {DependencyPath(this)};
    std::vector<DependencyPath> NewDependencyPaths;

    while (!DependencyPaths.empty()) {
      for (auto P : DependencyPaths) {
        for (auto& Dependency : P.back()->DependsOn) {
          if (Dependency.first == Target) {
            P.add(Target);
            return P;
          }

          if (CrossedModules.find(Dependency.first) == CrossedModules.end()) {
            DependencyPath NewPath = P;
            NewPath.add(Dependency.first);
            NewDependencyPaths.push_back(NewPath);
            CrossedModules.insert(Dependency.first);
          }
        }
      }
      DependencyPaths = NewDependencyPaths;
      NewDependencyPaths.clear();
    }

    return DependencyPath();
  }

  const std::map<const Module*, DependencyData>& getDependencyInformation() const {
    return DependsOn;
  };

  std::vector<const Module*> getDependsOn() const {
    std::vector<const Module*> Result;
    for (auto& pair : DependsOn) {
      Result.push_back(pair.first);
    }
    return Result;
  }

  const std::vector<Header>& getHeaders() const {
    return Headers;
  }

};


#endif //CYCLEDETECT_MODULE_H
