#include "HeaderInclude.h"

#include "Module.h"

bool HeaderInclude::parse(const std::string &str, const IncludePaths &Includes) {
  File = StringCache::ID();
  // Extraction of a sub-match
  static const std::regex base_regex("[\\s]*#[\\s]*include[\\s]*[<\"](\\S+)[>\"][\\s\\S]*");
  std::smatch base_match;

  if (!std::regex_match(str, base_match, base_regex))
    return false;

  std::string FileName = base_match[1];
  std::string FilePath = Includes.findFile(FileName);

  if (FilePath.empty()) {
    return false;
  }

  File = StringCache::get()[FilePath];
  return true;
}

const Header &HeaderInclude::getHeader() const {
  assert(TargetID.valid());
  return DependingModule->getHeader(TargetID);
}

void HeaderInclude::setDependingModule(const Module *M) {
  DependingModule = M;
  TargetID = M->getHeaderID(File);
  assert(TargetID.valid());
}
