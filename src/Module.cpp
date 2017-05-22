#include "Module.h"

void Module::updateName() {
  Name = Path.str();
  if (hasEnding(Name, "/interface")) {
    Name = Name.substr(0, Name.size() - std::string("/interface").size());
  }

  Name.erase(std::remove_if(Name.begin(), Name.end(), [](char c){return !std::isalnum(c);}), Name.end());
}

void Module::parseDirectory(const std::string &DirectoryPath,
                            const IncludePaths &GivenIncludePaths) {
  Path = normalizePath(DirectoryPath);
  updateName();

  IncludePaths UsedIncludePaths = GivenIncludePaths;
  UsedIncludePaths.addPath(DirectoryPath);

  using namespace boost::filesystem;
  directory_iterator File(DirectoryPath), eod;
  while (File != eod) {
    if (File->status().type() == boost::filesystem::file_type::regular_file) {
      std::string FilePath = File->path().string();
      Header header(FilePath, UsedIncludePaths);
      Headers.push_back(header);
    }
    File++;
  }
}

void Module::resolveDependencies(const std::vector<Module> &AllModules) {
  for (auto& Header : Headers) {
    for (auto& Include : Header.getIncludedHeaders()) {
      std::string HeaderPath = Include.getFile().str();

      for (auto& Module : AllModules) {
        if (Module.hasHeader(HeaderPath)) {
          Include.setDependingModule(&Module);
          if (&Module != this) {
            auto I = DependsOn.find(&Module);
            DependsOn[&Module].weight += 1;
          }
          break;
        }
      }

    }
  }
}

DependencyPath Module::getPathTo(const Module *Target) const {
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

std::vector<const Module *> Module::getDependsOn() const {
  std::vector<const Module*> Result;
  for (auto& pair : DependsOn) {
    Result.push_back(pair.first);
  }
  return Result;
}

std::string Module::getShortPath() const {
  std::string Result = getPath().str();
  if (hasEnding(Result, "/interface")) {
    Result = Result.substr(0, Result.size() - std::string("/interface").size());
  }
  return Result;
}
