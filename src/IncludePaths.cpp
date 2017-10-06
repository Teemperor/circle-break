#include "IncludePaths.h"
#include "FileSystemCache.h"

std::string IncludePaths::findFile(const std::string &PathStr) const {
  using namespace boost;

  filesystem::path Path(PathStr);

  if (Path.is_absolute()) {
    return Path.normalize().string();
  } else {
    for (const std::string& IncludePath : Paths) {
      filesystem::path LookupPath = IncludePath / Path;
      std::string Result = LookupPath.normalize().string();

      if (FileSystemCache::get().fileExists(Result)) {
        // Remove the annoying leading "./" on paths...
        if (Result.size() > 2 && Result[0] == '.' && Result[1] == '/') {
          return Result.substr(2);
        }
        return Result;
      }
    }
  }
  return std::string();
}

void IncludePaths::addPath(std::string PathStr) {
  using namespace boost;

  filesystem::path Path(PathStr);

  if (Path.is_relative()) {
    Path = boost::filesystem::current_path() / Path;
  }
  PathStr = Path.normalize().string();

  if (std::find(Paths.begin(), Paths.end(), Path) == Paths.end()) {
    Paths.push_back(PathStr);
  }
}
