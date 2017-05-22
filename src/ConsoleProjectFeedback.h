
#ifndef CIRCLE_BREAK_CONSOLEPROJECTFEEDBACK_H
#define CIRCLE_BREAK_CONSOLEPROJECTFEEDBACK_H

#include "Project.h"

class ConsoleProjectFeedback : public ProjectFeedback {
  int scanned = 0;
  int linked = 0;
  int parsed = 0;

public:
  ConsoleProjectFeedback() = default;

  virtual void startParsing() override {}

  virtual void startLinking() override {
    std::cout << std::endl;
  }

  virtual void startScanning() override {
    std::cout << std::endl;
  }

  virtual void startParsingModule(const Module& M) override {
    parsed++;
    std::cout << "\r[" << parsed << "/?] Parsing: "
              << M.getName() << "...                                                                                        ";
  }

  virtual void stopParsingModule(const Module& M) override {}
  virtual void startLinkingModule(const Module& M) override {
    linked++;
    std::cout << "\r[" << linked << "/" << parsed << "] Linking: "
              << M.getName() << "...                                                                                        ";
  }

  virtual void stopLinkingModule(const Module& M) override {}
  virtual void startScanningModule(const Module& M) override {
    scanned++;
    std::cout << "\r[" << scanned << "/" << parsed << "] Scanning: "
              << M.getName() << "...                                                                                        ";
  }

  virtual void stopScanningModule(const Module& M) override {}
};

#endif //CIRCLE_BREAK_CONSOLEPROJECTFEEDBACK_H
