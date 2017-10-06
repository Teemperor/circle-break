
#ifndef CIRCLE_BREAK_CONSOLEPROJECTFEEDBACK_H
#define CIRCLE_BREAK_CONSOLEPROJECTFEEDBACK_H

#include "Project.h"

class ConsoleProjectFeedback : public NoFeedback {
  int scanned = 0;
  int linked = 0;
  int parsed = 0;

public:
  ConsoleProjectFeedback() = default;

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

  virtual void startLinkingModule(const Module& M) override {
    linked++;
    std::cout << "\r[" << linked << "/" << parsed << "] Linking: "
              << M.getName() << "...                                                                                        ";
  }

  virtual void startScanningModule(const Module& M) override {
    scanned++;
    std::cout << "\r[" << scanned << "/" << parsed << "] Scanning: "
              << M.getName() << "...                                                                                        ";
  }

};

#endif //CIRCLE_BREAK_CONSOLEPROJECTFEEDBACK_H
