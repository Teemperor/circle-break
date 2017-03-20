#include "HeaderInclude.h"

bool HeaderInclude::parse(const std::string &str, const IncludePaths &Includes) {
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
