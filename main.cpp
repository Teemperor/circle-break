#include <iostream>
#include <Project.h>
#include <chrono>
#include <HtmlReport.h>

class ConsoleProjectFeedback : public ProjectFeedback {
  int scanned = 0;
  int linked = 0;
  int parsed = 0;

public:
  ConsoleProjectFeedback() = default;


  virtual void startParsing() override {
  }

  virtual void startLinking() override {
    std::cout << std::endl;

  }

  virtual void startScanning() override {
    std::cout << std::endl;
  }

  virtual void startParsingModule(const Module& M) override {
    parsed++;
    std::cout << "\r[" << parsed << "/?] Parsing: "
        << M.getName() << "...                                                                                        ";
  }

  virtual void stopParsingModule(const Module& M) override {};
  virtual void startLinkingModule(const Module& M) override {
    linked++;
    std::cout << "\r[" << linked << "/" << parsed << "] Linking: "
        << M.getName() << "...                                                                                        ";
  }

  virtual void stopLinkingModule(const Module& M) override {};
  virtual void startScanningModule(const Module& M) override {
    scanned++;
    std::cout << "\r[" << scanned << "/" << parsed << "] Scanning: "
        << M.getName() << "...                                                                                        ";
  }

  virtual void stopScanningModule(const Module& M) override {};
};

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

  ConsoleProjectFeedback* Feedback = nullptr;
  if (!Silent)
    Feedback = new ConsoleProjectFeedback();

  Project project(".", Feedback);
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

  if (Feedback) delete Feedback;
}
