
#ifndef CIRCLE_BREAK_PROJECTFEEDBACK_H
#define CIRCLE_BREAK_PROJECTFEEDBACK_H

#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <iostream>

class Module;
class Header;

class ProjectFeedback {

  std::chrono::steady_clock::time_point lastPrint;
  int milliPrintInterval = 10;

public:
  virtual ~ProjectFeedback() {
    lastPrint = std::chrono::steady_clock::time_point::min();
  }
  virtual void startParsing() = 0;
  virtual void startLinking() = 0;
  virtual void startScanning() = 0;

  virtual void startParsingModule(const Module& M) = 0;
  virtual void stopParsingModule(const Module& M) = 0;
  virtual void startLinkingModule(const Module& M) = 0;
  virtual void stopLinkingModule(const Module& M) = 0;
  virtual void startScanningModule(const Module& M) = 0;
  virtual void stopScanningModule(const Module& M) = 0;

  virtual void startParsingHeader(const std::string& Path) = 0;
  virtual void stopParsingHeader(const Header& H) = 0;
  virtual void startLinkingHeader(const Header& H) = 0;
  virtual void stopLinkingHeader(const Header& H) = 0;

  virtual void reportError(const std::string& Error) = 0;

  bool tryPrint() {
    auto now = std::chrono::steady_clock::now();
    auto diff = now - lastPrint;
    if (std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() >= milliPrintInterval) {
      lastPrint = now;
      return true;
    }
    return false;
  }

  std::string padString(const std::string& str, int size, bool right = false) {
    std::stringstream ss;
    ss << std::setw(size);
    if (!right)
      ss << std::left;
    ss << str;
    return ss.str();
  }

  std::string getProgressString(int current, int max);
};

class NoFeedback : public ProjectFeedback {
public:
  virtual ~NoFeedback() = default;
  virtual void startParsing() {};
  virtual void startLinking() {};
  virtual void startScanning() {};

  virtual void startParsingModule(const Module& M) {};
  virtual void stopParsingModule(const Module& M) {};
  virtual void startLinkingModule(const Module& M) {};
  virtual void stopLinkingModule(const Module& M) {};
  virtual void startScanningModule(const Module& M) {};
  virtual void stopScanningModule(const Module& M) {};

  virtual void startParsingHeader(const std::string& Path) {};
  virtual void stopParsingHeader(const Header& H) {};
  virtual void startLinkingHeader(const Header& H) {};
  virtual void stopLinkingHeader(const Header& H) {};

  virtual void reportError(const std::string& Error) {
    std::cerr << Error << std::endl;
  };
};

#endif //CIRCLE_BREAK_PROJECTFEEDBACK_H
