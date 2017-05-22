

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

class NoFeedback : public ProjectFeedback {
public:
  virtual ~NoFeedback() = default;
  virtual void startParsing() {}
  virtual void startLinking() {}
  virtual void startScanning() {}

  virtual void startParsingModule(const Module& M) {}
  virtual void stopParsingModule(const Module& M) {}
  virtual void startLinkingModule(const Module& M) {}
  virtual void stopLinkingModule(const Module& M) {}
  virtual void startScanningModule(const Module& M) {}
  virtual void stopScanningModule(const Module& M) {}
};

class Project {

protected:
  std::vector<Module> Modules;
  ProjectFeedback& Feedback;

  void link() {
    Feedback.startLinking();
    for (auto& Module : Modules) {
      Feedback.startLinkingModule(Module);
      Module.resolveDependencies(Modules);
      Feedback.stopLinkingModule(Module);
    }
  }

public:
  Project(ProjectFeedback& Feedback);

  std::vector<DependencyPath> getCycles() const;

  const std::vector<Module>& getModules() const {
    return Modules;
  }
};

class SCRAMProject : public Project {
public:
  SCRAMProject(const std::string& path, ProjectFeedback& Feedback);
};

class LibraryProject : public Project {
public:
  LibraryProject(const std::string& path, ProjectFeedback& Feedback) : Project(Feedback) {
    Module module("main");
    Feedback.startParsingModule(module);

    using namespace boost;
    filesystem::recursive_directory_iterator dir(path), end;

    IncludePaths IncPaths(false);
    IncPaths.addPath(path);

    Feedback.startParsing();
    while (dir != end) {
      module.parseDirectory(dir->path().string(), IncPaths);
      ++dir;
    }

    Feedback.stopParsingModule(module);

    Modules.push_back(module);
    link();
  }
};


#endif //CYCLEDETECT_PROJECT_H
