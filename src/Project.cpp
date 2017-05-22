

#include "Project.h"

Project::Project(ProjectFeedback *Feedback)
    : Feedback(Feedback) {
}

std::vector<DependencyPath> Project::getCycles() const {
  if (Feedback) Feedback->startScanning();
  std::vector<DependencyPath> Result;
  for (auto& Module : Modules) {
    if (Feedback) Feedback->startScanningModule(Module);
    for (auto& D : Module.getDependsOn()) {
      if (auto P = D->getPathTo(&Module)) {
        P.add(D);
        Result.push_back(P);
      }
    }
    if (Feedback) Feedback->stopScanningModule(Module);
  }

  auto end = std::unique(Result.begin(), Result.end(), [](DependencyPath& P1, DependencyPath& P2){
    return P1.similar(P2);
  });
  Result.erase(end, Result.end());

  return Result;
}

SCRAMProject::SCRAMProject(const std::string &path, ProjectFeedback *Feedback)
    : Project(Feedback) {

  using namespace boost;
  filesystem::recursive_directory_iterator dir(path), end;

  IncludePaths IncPaths(false);
  IncPaths.addPath(path);

  if (Feedback) Feedback->startParsing();
  while (dir != end) {
    if (dir->path().filename() == "interface") {
      Module module(dir->path().string());
      if (Feedback) Feedback->startParsingModule(module);
      module.parseDirectory(dir->path().string(), IncPaths);
      Modules.push_back(module);
      if (Feedback) Feedback->stopParsingModule(module);
    }

    ++dir;
  }
  link();
}
