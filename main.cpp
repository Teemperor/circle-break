#include <iostream>
#include <Project.h>


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

void writeHtmlHeader(std::ofstream& stream) {
  stream << "<!DOCTYPE html>\n"
         << "<html lang=\"en\">\n"
         << "  <head>\n"
         << "    <meta charset=\"utf-8\">\n"
         << "    <title>Cycles in project</title>\n"
         << "  </head>\n"
         << "  <body>\n";
}

std::string getCycleString(const DependencyPath& Cycle) {
  std::stringstream cycleString;
  cycleString << "<pre>";
  for (std::size_t i = 0; i < Cycle.length() - 1; ++i) {
    auto Node = Cycle.at(i);
    auto NextNode = Cycle.at((i + 1) % Cycle.length());
    auto weight = Node->getDependencyInformation().at(NextNode).weight;
    cycleString << Node->getName() << " &rarr; ";
  }
  cycleString << Cycle.back()->getName() << "</pre>\n";

  return cycleString.str();
}

void writeHtmlFooter(std::ofstream& stream) {
  stream << "  </body>\n"
         << "</html>";
}

void writeSingleCycle(const DependencyPath& Cycle, std::size_t CycleIndex) {
  std::ofstream cycleFile(std::string("cycle") + std::to_string(CycleIndex) + ".cycle.html");
  assert(cycleFile.good());
  writeHtmlHeader(cycleFile);

  cycleFile << "<a href=\"cycle_report.cycle.html\">Back</a>\n";
  cycleFile << "<h1>Cyclic dependency " << CycleIndex << "</h1>\n";
  cycleFile << "<p>Cyclic dependency across following modules: </p>\n";
  cycleFile << "<p style=\"font-weight: bold; \">" << getCycleString(Cycle) << "</p>\n";
  cycleFile << "<p>Break one of the following dependencies to resolve this cyclic dependency:</p>\n";

  std::map<std::size_t, std::pair<const Module*, const Module*> > EdgesByWeight;
  for (std::size_t i = 0; i < Cycle.length() - 1; ++i) {
    auto Node = Cycle.at(i);
    auto NextNode = Cycle.at((i + 1) % Cycle.length());
    auto weight = Node->getDependencyInformation().at(NextNode).weight;
    EdgesByWeight.emplace(weight, std::make_pair(Node, NextNode));
  }

  size_t Index = 1;
  for (std::pair<const std::size_t, std::pair<const Module*, const Module*> >& Dep : EdgesByWeight) {
    cycleFile << "<div style=\"\n"
        "    background-color: beige;\n"
        "    border-color: black;\n"
        "    padding-left: 1em;\n"
        "    border-style: dotted;\n"
        "    margin: 1em;\">\n";
    cycleFile << "<h2>Dependency from " << Dep.second.first->getName() << " &rarr; " << Dep.second.second->getName() << "</h2>\n";
    cycleFile << "Dependency created by following includes in " << Dep.second.first->getName() << ":<p>\n";

    for (auto& Header : Dep.second.first->getHeaders()) {
      for (auto& Inc : Header.getIncludedHeaders()) {
        if (Inc.getDependingModule() == Dep.second.second) {
          cycleFile << "<p>In file <span style=\"font-family: monospace; font-weight: bold;\">"
                    << Header.getPath().str() << ":" << Inc.getLineNumber() << "</span>" <<
               ": <span style=\"font-family: monospace; font-weight: bold; color:#800000;\">#include </span>" <<
               "<span style=\"font-family: monospace; font-weight: bold; color:#339933;\">\"" << Inc.getFile().str()
                    << "\"</span></p>\n";
        }
      }
    }
    cycleFile << "</div>\n";
    Index++;
  }

  writeHtmlFooter(cycleFile);
}

void writeCycleReport(const std::vector<DependencyPath>& Cycles) {
  std::ofstream reportFile("cycle_report.cycle.html");
  assert(reportFile.good());
  writeHtmlHeader(reportFile);

  size_t CycleIndex = 1;
  for (auto& Cycle : Cycles) {
    reportFile << "<div style=\"\n"
        "    background-color: beige;\n"
        "    border-color: black;\n"
        "    padding-left: 1em;\n"
        "    border-style: dotted;\n"
        "    margin: 1em;\">\n";
    reportFile << "<h1>Cyclic dependency " << CycleIndex << "</a></h1>\n";
    reportFile << "<p>Cyclic dependency across following modules: </p>\n";
    reportFile << "<a href=\"cycle" << CycleIndex << ".cycle.html\">" << getCycleString(Cycle) << "</a>";


    writeSingleCycle(Cycle, CycleIndex);

    reportFile << "</div>\n";

    CycleIndex++;
  }

  writeHtmlFooter(reportFile);
}

int main() {
  ConsoleProjectFeedback Feedback;
  Project project(".", &Feedback);

  auto Cycles = project.getCycles();

  writeCycleReport(Cycles);


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