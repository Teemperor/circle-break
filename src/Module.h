#ifndef CYCLEDETECT_MODULE_H
#define CYCLEDETECT_MODULE_H

#include <boost/filesystem.hpp>
#include <iostream>

class Module {

public:
  Module() {
  }

  void parseDirectory(const std::string& DirectoryPath) {
    using namespace boost::filesystem;
    directory_iterator files(DirectoryPath), eod;
    while (files != eod) {
      path p = "include/.." / *files / "moo";
      std::string s = p.normalize().string();
      std::cout << s << std::endl;
      files++;
    }
  }

};


#endif //CYCLEDETECT_MODULE_H
