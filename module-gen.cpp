#include <iostream>
#include <Project.h>
#include <DependencyTree.h>

void printHelp() {
  std::cerr << "module-gen [FLAGS...]\n";
  std::cerr << " -IPATH, -I PATH,  Adds a custom include path for parsing.\n"
               "    -I=PATH\n";
  std::cerr << " --help, -h        Shows this help.\n";
}


class PerHeaderProjectFeedback : public NoFeedback {
  int parsed = 0;
  int linked = 0;

public:
  virtual void startLinkingHeader(const Header &H) override {
    linked++;
    if (tryPrint())
      std::cout << "\r[" << getProgressString(linked, parsed) << "] Linking: "
              << padString(H.getPath().str(), 120);
  }

  virtual void startParsingHeader(const std::string& Path) override {
    parsed++;
    if (tryPrint())
      std::cout << "\r[" << parsed << "/?] Parsing: "
              << padString(Path, 120);
  }
};

template <typename T>
void printOutput(T& output, DependencyTree& Tree) {
  for (auto& Header : Tree.getHeaderOrder()) {
    output << Header.getPath().str() << "\n";
  }
}

int main(int argc, char **argv) {

  ProjectConfiguration Config;
  Config.setFeedback(new PerHeaderProjectFeedback());

  std::string outputFile = "-";

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
    } else if (arg.find("-I=") != arg.npos) {
      Config.addIncludePath(arg.substr(std::string("-I=").size()));
    } else if (arg.find("-I") != arg.npos) {
      Config.addIncludePath(arg.substr(std::string("-I").size()));
    } else if (arg == "--help" || arg == "-h") {
      printHelp();
      return 0;
    } else if (arg == "-o") {
      argi++;
      if (argi == argc) {
        std::cerr << "-o without following output file!\n";
        return 1;
      }
      outputFile = argv[argi];
    } else {
      std::cerr << "Unknown argument: " << arg << std::endl;
    }
  }

  if (NextIsIncludePath) {
    std::cerr << "Missing include path at the end of invocation.\n";
  }

  std::ofstream output;
  if (outputFile != "-") {
    output.open(outputFile);
    if (!output.good()) {
      std::cerr << "Can't open output file '" << outputFile << "'!\n";
      return 1;
    }
  }

  LibraryProject Project(".", Config);

  DependencyTree Tree(Project, Config.getFeedback());

  if (outputFile != "-") {
    printOutput(output, Tree);
    output.close();
  } else {
    printOutput(std::cout, Tree);
  }

}
