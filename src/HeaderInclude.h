#ifndef CYCLEDETECT_HEADERINCLUDE_H
#define CYCLEDETECT_HEADERINCLUDE_H

#include <string>
#include <regex>

#include "StringCache.h"
#include "IncludePaths.h"

class Module;

class HeaderInclude {

  StringCache::ID File;
  unsigned LineNumber;
  const Module* DependingModule = nullptr;

public:
  HeaderInclude() {
  }

  bool valid() const {
    return File.valid();
  }

  StringCache::ID getFile() const {
    assert(valid());
    return File;
  }

  bool parse(const std::string& str, const IncludePaths& Includes);

  void setDependingModule(const Module* M) {
    DependingModule = M;
  }

  const Module* getDependingModule() const {
    return DependingModule;
  }

  void setLine(unsigned int NewLineNumber) {
    LineNumber = NewLineNumber;
  }

  unsigned getLineNumber() const {
    return LineNumber;
  }
};


#endif //CYCLEDETECT_HEADERINCLUDE_H
