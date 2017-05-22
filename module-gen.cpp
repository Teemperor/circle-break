#include <iostream>
#include <Project.h>

void printHelp() {
  std::cerr << "module-gen [FLAGS...]\n";
  std::cerr << " -IPATH, -I PATH,  Adds a custom include path for parsing.\n"
               "    -I=PATH\n";
  std::cerr << " --help, -h        Shows this help.\n";
}

int main(int argc, char **argv) {

  ProjectConfiguration Config;

  bool NextIsIncludePath = false;

  for (int argi = 1; argi < argc; argi++) {
    std::string arg = argv[argi];
    if (NextIsIncludePath) {
      NextIsIncludePath = false;
      Config.addIncludePath(arg);
      continue;
    }

    if (arg == "-I") {
      NextIsIncludePath = true;
    } else if (arg.find("-I=")) {
      Config.addIncludePath(arg.substr(std::string("-I=").size()));
    } else if (arg.find("-I")) {
      Config.addIncludePath(arg.substr(std::string("-I").size()));
    } else if (arg == "--help" || arg == "-h") {
      printHelp();
      return 0;
    } else {
      std::cerr << "Unknown argument: " << arg << std::endl;
    }
  }

  if (NextIsIncludePath) {
    std::cerr << "Missing include path at the end of invocation.\n";
  }

  LibraryProject Project(".", Config);

}
