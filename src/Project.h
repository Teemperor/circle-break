

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

protected:
  std::vector<Module> Modules;
  ProjectFeedback* Feedback;

  IncludePaths IncPaths;

  void link() {
    if (Feedback) Feedback->startLinking();
    for (auto& Module : Modules) {
      if (Feedback) Feedback->startLinkingModule(Module);
      Module.resolveDependencies(Modules);
      if (Feedback) Feedback->stopLinkingModule(Module);
    }
  }

public:
  Project(const std::string& path, ProjectFeedback* Feedback = nullptr);

  std::vector<DependencyPath> getCycles() const;

  const std::vector<Module>& getModules() const {
    return Modules;
  }
};

class SCRAMProject : public Project {
public:
  SCRAMProject(const std::string& path, ProjectFeedback* Feedback = nullptr);
};


#endif //CYCLEDETECT_PROJECT_H
