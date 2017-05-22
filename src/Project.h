

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

  void link() {
    if (Feedback) Feedback->startLinking();
    for (auto& Module : Modules) {
      if (Feedback) Feedback->startLinkingModule(Module);
      Module.resolveDependencies(Modules);
      if (Feedback) Feedback->stopLinkingModule(Module);
    }
  }

public:
  Project(ProjectFeedback* Feedback = nullptr);

  std::vector<DependencyPath> getCycles() const;

  const std::vector<Module>& getModules() const {
    return Modules;
  }
};

class SCRAMProject : public Project {
public:
  SCRAMProject(const std::string& path, ProjectFeedback* Feedback = nullptr);
};

class LibraryProject : public Project {
public:
  LibraryProject(const std::string& path, ProjectFeedback* Feedback = nullptr) : Project(Feedback) {
    Module module("main");
    if (Feedback) Feedback->startParsingModule(module);

    using namespace boost;
    filesystem::recursive_directory_iterator dir(path), end;

    IncludePaths IncPaths(false);
    IncPaths.addPath(path);

    if (Feedback) Feedback->startParsing();
    while (dir != end) {
      module.parseDirectory(dir->path().string(), IncPaths);
      ++dir;
    }

    if (Feedback) Feedback->stopParsingModule(module);

    Modules.push_back(module);
    link();
  }
};


#endif //CYCLEDETECT_PROJECT_H
