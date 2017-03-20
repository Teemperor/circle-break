#include "FileSystemCache.h"

bool FileSystemCache::exists(const std::string &LookupPath) {
  auto Iter = ExistsCache.find(LookupPath);
  if (Iter == ExistsCache.end()) {
    bool FileExists = boost::filesystem::exists(LookupPath);
    ExistsCache[LookupPath] = FileExists;
    return FileExists;
  }

  return Iter->second;
}

FileSystemCache &FileSystemCache::get() {
  static FileSystemCache Instance;
  return Instance;
}
