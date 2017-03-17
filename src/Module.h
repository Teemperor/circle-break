#ifndef CYCLEDETECT_MODULE_H
#define CYCLEDETECT_MODULE_H

#include <boost/filesystem.hpp>
#include <iostream>

#include "IncludePaths.h"
#include "Header.h"

class Module {

  std::vector<Header> Headers;

public:
  Module() {
  }

  void parseDirectory(const std::string& DirectoryPath, const IncludePaths& GivenIncludePaths) {
    IncludePaths UsedIncludePaths = GivenIncludePaths;
    UsedIncludePaths.addPath(DirectoryPath);

    using namespace boost::filesystem;
    directory_iterator files(DirectoryPath), eod;
    while (files != eod) {
      std::string file = files->path().string();
      Header header(file, UsedIncludePaths);
      Headers.push_back(header);
      files++;
    }
  }

  const std::vector<Header>& getHeaders() const {
    return Headers;
  }

};


#endif //CYCLEDETECT_MODULE_H
