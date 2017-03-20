#ifndef CYCLEDETECT_FILESYSTEMCACHE_H
#define CYCLEDETECT_FILESYSTEMCACHE_H

#include <boost/filesystem.hpp>
#include <unordered_map>

class FileSystemCache {

  std::unordered_map<std::string, bool> ExistsCache;

public:

  static FileSystemCache& get();

  bool exists(const std::string& LookupPath);

};


#endif //CYCLEDETECT_FILESYSTEMCACHE_H
