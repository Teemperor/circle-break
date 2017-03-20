

#ifndef CYCLEDETECT_PROJECT_H
#define CYCLEDETECT_PROJECT_H


#include <boost/filesystem.hpp>
#include <vector>

#include "Module.h"

class ProjectFeedback {
public:
  virtual ~ProjectFeedback() = default;


  virtual void startParsing() = 0;
  virtual void startLinking() = 0;
  virtual void startScanning() = 0;

  virtual void startParsingModule(const Module& M) = 0;
  virtual void stopParsingModule(const Module& M) = 0;
  virtual void startLinkingModule(const Module& M) = 0;
  virtual void stopLinkingModule(const Module& M) = 0;
  virtual void startScanningModule(const Module& M) = 0;
  virtual void stopScanningModule(const Module& M) = 0;
};

class Project {

  std::vector<Module> Modules;
  ProjectFeedback* Feedback;

public:
  Project(const std::string& path, ProjectFeedback* Feedback = nullptr) : Feedback(Feedback) {
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

    if (Feedback) Feedback->startLinking();
    for (auto& Module : Modules) {
      if (Feedback) Feedback->startLinkingModule(Module);
      Module.resolveDependencies(Modules);
      if (Feedback) Feedback->stopLinkingModule(Module);
    }

  }

  std::vector<DependencyPath> getCycles() const {
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

  const std::vector<Module>& getModules() const {
    return Modules;
  }


};


#endif //CYCLEDETECT_PROJECT_H
