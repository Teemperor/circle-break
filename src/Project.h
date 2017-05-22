

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

class ProjectConfiguration {
  std::vector<std::string> AdditionalIncludePaths;
  std::unique_ptr<ProjectFeedback> Feedback;
public:
  explicit ProjectConfiguration() : Feedback(new NoFeedback()){
  }

  explicit ProjectConfiguration(ProjectFeedback *Feedback) : Feedback(Feedback){
  }

  const std::vector<std::string>& getAdditionalIncludePaths() const {
    return AdditionalIncludePaths;
  }

  void addIncludePath(const std::string& Path) {
    AdditionalIncludePaths.push_back(Path);
  }

  void setFeedback(ProjectFeedback *F) {
    Feedback.reset(F);
  }

  ProjectFeedback &getFeedback() {
    return *Feedback;
  }

};

class Project {

protected:
  std::vector<Module> Modules;
  ProjectFeedback& Feedback;
  ProjectConfiguration& Config;

  void link() {
    Feedback.startLinking();
    for (auto& Module : Modules) {
      Feedback.startLinkingModule(Module);
      Module.resolveDependencies(Modules);
      Feedback.stopLinkingModule(Module);
    }
  }

public:
  Project(ProjectConfiguration& Config);

  std::vector<DependencyPath> getCycles() const;

  const std::vector<Module>& getModules() const {
    return Modules;
  }
};

class SCRAMProject : public Project {
public:
  SCRAMProject(const std::string& path, ProjectConfiguration& Config);
};

class LibraryProject : public Project {
public:
  LibraryProject(const std::string& path, ProjectConfiguration& Config) : Project(Config) {
    Module module("main");
    Feedback.startParsingModule(module);

    using namespace boost;
    filesystem::recursive_directory_iterator dir(path), end;

    IncludePaths IncPaths;
    IncPaths.addPath(path);
    IncPaths.addPaths(Config.getAdditionalIncludePaths());

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
