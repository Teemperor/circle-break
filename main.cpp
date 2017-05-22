#include <iostream>
#include <Project.h>
#include <chrono>
#include <HtmlReport.h>
#include <ConsoleProjectFeedback.h>

void printHelp() {
  std::cerr << "circle-break [FLAGS...]\n";
  std::cout << "Scans the SCRAM project in the current working directory.\n";
  std::cerr << " --html        Generate a HTML report in the local directory.\n"
               "               The report files all have a *.cycle.html suffix.\n";
  std::cerr << " --help, -h    Shows this help.\n";
  std::cerr << " --silent, -s  Don't print information to stdout.\n";
}

int main(int argc, char **argv) {
  bool GenerateHtml = false;
  bool Silent = false;

  for (int argi = 1; argi < argc; argi++) {
    std::string arg = argv[argi];
    if (arg == "--html") {
      GenerateHtml = true;
    } else if (arg == "--help" || arg == "-h") {
      printHelp();
      return 0;
    } else if (arg == "--silent" || arg == "-s") {
      Silent = true;
    } else {
      std::cerr << "Unknown argument: " << arg << std::endl;
    }
  }

  ProjectConfiguration Config;

  if (!Silent)
    Config.setFeedback(new ConsoleProjectFeedback());

  Project project(Config);
  std::vector<DependencyPath> Cycles = project.getCycles();


  if (GenerateHtml) {
    HtmlReport Report;
    Report.handleProject(project, Cycles);
  }

  if (!Silent) {
    std::cout << std::endl;
    std::cout << "Found " << Cycles.size() << " cyclic dependencies:\n";
    unsigned Index = 1;
    for (auto& Cycle : Cycles) {
      std::cout << "No. " << Index << ": ";
      for (std::size_t i = 0; i < Cycle.length() - 1; ++i) {
        auto Node = Cycle.at(i);
        auto NextNode = Cycle.at(i + 1);
        auto weight = Node->getDependencyInformation().at(NextNode).weight;
        std::cout << Node->getShortPath() << " =" << weight << "=> ";
      }
      std::cout << Cycle.back()->getShortPath() << "\n";

      Index++;
    }
  }
}
