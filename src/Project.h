

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
  Project(const std::string& path, ProjectFeedback* Feedback = nullptr);

  std::vector<DependencyPath> getCycles() const;

  const std::vector<Module>& getModules() const {
    return Modules;
  }


};


#endif //CYCLEDETECT_PROJECT_H
