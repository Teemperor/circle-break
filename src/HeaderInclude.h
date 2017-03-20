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

  bool parse(const std::string& str, const IncludePaths& Includes) {
    File = StringCache::ID();
    // Extraction of a sub-match
    static const std::regex base_regex("[\\s]*#include[\\s]*[<\"](\\S+)[>\"][\\s\\S]*");
    std::smatch base_match;

    if (!std::regex_match(str, base_match, base_regex))
      return false;

    std::string FileName = base_match[1];
    std::string FilePath = Includes.findFile(FileName);

    if (FilePath.empty())
      return false;

    File = StringCache::get()[FilePath];
    return true;
  }

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
