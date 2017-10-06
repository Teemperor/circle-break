

#ifndef CYCLEDETECT_PROJECT_H
#define CYCLEDETECT_PROJECT_H


#include <boost/filesystem.hpp>
#include <vector>

#include "Module.h"
#include "ProjectFeedback.h"

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
      Module.resolveDependencies(Modules, Feedback);
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

    IncludePaths IncPaths;
    IncPaths.addPath(path);
    IncPaths.addPaths(Config.getAdditionalIncludePaths());

    Feedback.startParsing();

    module.parseDirectory(path, IncPaths, Feedback);

    using namespace boost;
    filesystem::recursive_directory_iterator dir(path), end;
    while (dir != end) {
      if (dir->status().type() == boost::filesystem::file_type::directory_file) {
        module.parseDirectory(dir->path().string(), IncPaths, Feedback);
      }
      ++dir;
    }

    Feedback.stopParsingModule(module);

    Modules.push_back(module);
    link();
  }
};


#endif //CYCLEDETECT_PROJECT_H
