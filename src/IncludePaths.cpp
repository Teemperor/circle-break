#include "IncludePaths.h"

std::string IncludePaths::findFile(const std::string &PathStr) const {
  using namespace boost;

  filesystem::path Path(PathStr);

  if (Path.is_absolute()) {
    return Path.normalize().string();
  } else {
    for (const std::string& IncludePath : Paths) {
      filesystem::path LookupPath = IncludePath / Path;
      if (filesystem::exists(LookupPath)) {
        std::string Result = LookupPath.normalize().string();
        if (Result.size() > 2 && Result[0] == '.' && Result[1] == '/') {
          return Result.substr(2);
        }
        return Result;
      }
    }
  }
  return std::string();
}
