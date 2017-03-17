

#ifndef CYCLEDETECT_PROJECT_H
#define CYCLEDETECT_PROJECT_H


#include <boost/filesystem.hpp>
#include <vector>

#include "Module.h"

class Project {

  std::vector<Module> Modules;

public:
  Project() {
   /* using namespace boost::filesystem;
    recursive_directory_iterator dir(DirectoryPath), end;
    while (dir != end) {
      dir->
          ++dir;
    } */
  }


};


#endif //CYCLEDETECT_PROJECT_H
