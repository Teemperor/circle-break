#include <iostream>
#include <Project.h>


class ConsoleProjectFeedback : public ProjectFeedback {
  int scanned = 0;
  int linked = 0;
  int parsed = 0;

public:
  ConsoleProjectFeedback() {

  }

  virtual void startParsingModule(const Module& M) override {
    parsed++;
    std::cout << "\rParsing " << parsed << ": " << M.getName() << "...";
    std::cout.flush();
  }

  virtual void stopParsingModule(const Module& M) override {
    std::cout << " OK!                                                                              ";
  }

  virtual void startLinkingModule(const Module& M) override {
    linked++;
    std::cout << "\r[" << linked << "/" << parsed << "] Linking " << parsed << ": " << M.getName() << "...";
    std::cout.flush();
  }

  virtual void stopLinkingModule(const Module& M) override {
    std::cout << " OK!                                                                              ";
  }

  virtual void startScanningModule(const Module& M) override {
    scanned++;
    std::cout << "\r[" << scanned << "/" << parsed << "]Scanning " << scanned << ": " << M.getName() << "...";
    std::cout.flush();
  }

  virtual void stopScanningModule(const Module& M) override {
    std::cout << " OK!                                                                              ";
  }
};

int main() {
  ConsoleProjectFeedback Feedback;
  Project project(".", &Feedback);

  auto Cycles = project.getCycles();

  std::ofstream report_file("cycle_report.cycle.html");
  assert(report_file.good());
  report_file << "<!DOCTYPE html>\n"
      << "<html lang=\"en\">\n"
      << "  <head>\n"
      << "    <meta charset=\"utf-8\">\n"
      << "    <title>Cycles in project</title>\n"
      << "  </head>\n"
      << "  <body>\n";


  size_t CycleIndex = 1;
  for (auto& Cycle : Cycles) {
    std::map<std::size_t, std::pair<const Module*, const Module*> > EdgesByWeight;

    report_file << "<h1><a href=\"cycle" << CycleIndex << ".cycle.html\">Cycle " << CycleIndex << "</a></h1>" << std::endl;
    std::stringstream cycleString;
    cycleString << "<pre>";
    for (std::size_t i = 0; i < Cycle.length() - 1; ++i) {
      auto Node = Cycle.at(i);
      auto NextNode = Cycle.at((i + 1) % Cycle.length());
      auto weight = Node->getDependencyInformation().at(NextNode).weight;
      cycleString << Node->getName() << " =|" << weight << "|=&gt; ";
      EdgesByWeight.emplace(weight, std::make_pair(Node, NextNode));
    }
    cycleString << Cycle.back()->getName() << "</pre>\n";

    report_file << cycleString.str();

    std::ofstream cycle_file(std::string("cycle") + std::to_string(CycleIndex) + ".cycle.html");
    assert(cycle_file.good());
    cycle_file << "<!DOCTYPE html>\n"
                << "<html lang=\"en\">\n"
                << "  <head>\n"
                << "    <meta charset=\"utf-8\">\n"
                << "    <title>Cycle " << CycleIndex << "</title>\n"
                << "  </head>\n"
                << "  <body>\n";

    cycle_file << "<a href=\"cycle_report.cycle.html\">Back</a>" << std::endl;
    cycle_file << "<h1>Cycle " << CycleIndex << "</h1>" << std::endl;

    size_t Index = 1;
    for (std::pair<const std::size_t, std::pair<const Module*, const Module*> >& Dep : EdgesByWeight) {
      cycle_file << "\n" << "<h2>link " << Dep.second.first->getName() << "=|" << Dep.first << "|=&gt;" << Dep.second.second->getName() << "</h2>" << std::endl;

      for (auto& Header : Dep.second.first->getHeaders()) {
        for (auto& Inc : Header.getIncludedHeaders()) {
          if (Inc.getDependingModule() == Dep.second.second) {
            cycle_file << "<p><pre>" << Header.getPath().str() << ":" << Inc.getLineNumber() << ": #include \"" << Inc.getFile().str() << "\"</pre></p>" << std::endl;
          }
        }
      }
      Index++;
    }
    CycleIndex++;


    cycle_file << "  </body>\n"
                << "</html>";
  }

  report_file << "  </body>\n"
              << "</html>";


/*
  std::cout << "digraph {\n";
  for (auto& Module : project.getModules()) {
    std::cout << "\n  // module " << Module.getName() << "\n";
    for (auto& Dependency : Module.getDependencyInformation()) {
      int weight = Dependency.second.weight;
      std::cout << "  " << Module.getName() << " -> " << Dependency.first->getName() << "[label=\"" << weight <<
                        "\",weight=\"" << weight << "\"]\n";
    }
  }
  std::cout << "}\n";
  */
}