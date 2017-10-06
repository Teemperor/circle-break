

#include "Project.h"

Project::Project(ProjectConfiguration& Config)
    : Feedback(Config.getFeedback()), Config(Config) {
}

std::vector<DependencyPath> Project::getCycles() const {
  Feedback.startScanning();
  std::vector<DependencyPath> Result;
  for (auto& Module : Modules) {
    Feedback.startScanningModule(Module);
    for (auto& D : Module.getDependsOn()) {
      if (auto P = D->getPathTo(&Module)) {
        P.add(D);
        Result.push_back(P);
      }
    }
    Feedback.stopScanningModule(Module);
  }

  auto end = std::unique(Result.begin(), Result.end(), [](DependencyPath& P1, DependencyPath& P2){
    return P1.similar(P2);
  });
  Result.erase(end, Result.end());

  return Result;
}

SCRAMProject::SCRAMProject(const std::string &path, ProjectConfiguration& Config)
    : Project(Config) {


  IncludePaths IncPaths;
  IncPaths.addPath(path);
  IncPaths.addPaths(Config.getAdditionalIncludePaths());

  Feedback.startParsing();
  using namespace boost;
  filesystem::recursive_directory_iterator dir(path), end;
  while (dir != end) {
    if (dir->path().filename() == "interface") {
      Module module(dir->path().string());
      Feedback.startParsingModule(module);
      module.parseDirectory(dir->path().string(), IncPaths, Feedback);
      Modules.push_back(module);
      Feedback.stopParsingModule(module);
    }

    ++dir;
  }
  link();
}
