#include <iostream>
#include <Project.h>
#include <chrono>


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

typedef std::unordered_map<const Module*, std::unordered_set<const Module*> > UsedModulesDict;

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

void writeFilterList(std::ofstream& stream, std::size_t uid, const Module& Module, Project& project,
                     const UsedModulesDict& UsableModules) {

  stream << "<h4 style=\"margin-top: 3em;\">Search headers/modules that can be included from "
         << Module.getShortPath() << ":</h4><p> Green color = can be included here. "
      "Red color = would cause another cylic dependency.\n"
      "Examples searches are 'DataFormats/TrackerCommon/interface/TrackerTopology.h' or 'DataFormats/TrackerCommon'</p>\n";
  stream << "<input type=\"text\" class=\"myInput\" id=\"myInput" << uid <<
           "\" onkeyup=\"filterFunc" << uid << "()\" "
           "placeholder=\"Search for usable modules...\" title=\"Type a module name\">";

  stream <<
      "<ul class=\"filterList\" id=\"myUL" << uid << "\">\n";

  auto& CurrentUsableModules = UsableModules.at(&Module);

  for (auto& ModuleIter : project.getModules()) {
    bool Usable = CurrentUsableModules.find(&ModuleIter) != CurrentUsableModules.end();
    stream << "<li style=\"display: none;\" class=\"" <<
            (Usable ? "good" : "bad") << "\">" << ModuleIter.getShortPath()
           << "</li>\n";
  }

  stream << "</ul>\n";

  stream <<
      "<script>\n"
      "function filterFunc" << uid << "() {\n"
      "    var input, filter, ul, li, a, i;\n"
      "    input = document.getElementById(\"myInput" << uid << "\");\n"
      "    path = input.value.toUpperCase();\n"
      "    path = path.trim();\n"
      "    if (path.endsWith(\".H\") || path.endsWith(\".HH\"))\n"
      "      if (path.lastIndexOf(\"/\") !== -1)\n"
      "        path = path.substring(0, path.lastIndexOf(\"/\"));\n"
      "    if (path.endsWith(\"/INTERFACE\"))\n"
      "      path = path.substring(0, path.length - \"/INTERFACE\".length);\n"
      "    ul = document.getElementById(\"myUL" << uid << "\");\n"
      "    li = ul.getElementsByTagName(\"li\");\n"
      "    for (i = 0; i < li.length; i++) {\n"
      "        if (path.length != 0 && li[i].innerHTML.toUpperCase().indexOf(path) > -1) {\n"
      "            li[i].style.display = \"\";\n"
      "        } else {\n"
      "            li[i].style.display = \"none\";\n"
      "        }\n"
      "    }\n"
      "}\n"
      "</script>\n";

}


void writeSingleCycle(const DependencyPath& Cycle, std::size_t CycleIndex,  Project& project,
                      const UsedModulesDict& UsableModules) {
  std::ofstream cycleFile(std::string("cycle") + std::to_string(CycleIndex) + ".cycle.html");
  assert(cycleFile.good());

  cycleFile << "<!DOCTYPE html>\n"
            << "<html lang=\"en\">\n"
            << "  <head>\n"
            << "<style>"
                "* {\n"
                "  box-sizing: border-box;\n"
                "}\n"
                "\n"
                ".myInput {\n"
                "  width: 100%;\n"
                "  font-size: medium;\n"
                "  padding: 0.3em;\n"
                "  border: 1px solid #ddd;\n"
                "  margin-bottom: 12px;\n"
                "}\n"
                "\n"
                ".filterList {\n"
                "  list-style-type: none;\n"
                "  padding: 0;\n"
                "  margin: 0;\n"
                "}\n"
                "\n"
                ".good { background-color: #e6ffcc; }\n"
                ".bad { background-color: #ffcccc; }\n"
                ".filterList li{\n"
                "  border: 1px solid #ddd;\n"
                "  margin-top: -1px; /* Prevent double borders */\n"
                "  padding: 12px;\n"
                "  text-decoration: none;\n"
                "  font-size: 18px;\n"
                "  color: black;\n"
                "  display: block\n"
                "}\n"
                "\n"
                "\n"
                "</style>"
            << "    <meta charset=\"utf-8\">\n"
            << "    <title>Cycles in project</title>\n"
            << "  </head>\n"
            << "  <body>\n";

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
    writeFilterList(cycleFile, Index, *Dep.second.first, project, UsableModules);
    cycleFile << "</div>\n";
    Index++;
  }

  writeHtmlFooter(cycleFile);
}

void writeCycleReport(const std::vector<DependencyPath>& Cycles,  Project& project,
                      const UsedModulesDict& UsableModules) {
  std::ofstream reportFile("cycle_report.cycle.html");
  assert(reportFile.good());
  writeHtmlHeader(reportFile);

  std::chrono::system_clock::time_point p = std::chrono::system_clock::now();

  std::time_t t = std::chrono::system_clock::to_time_t(p);
  reportFile << "<h5> Generated on " << std::ctime(&t) << "</h5>\n";

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


    writeSingleCycle(Cycle, CycleIndex, project, UsableModules);

    reportFile << "</div>\n";

    CycleIndex++;
  }

  writeHtmlFooter(reportFile);
}

int main() {
  ConsoleProjectFeedback Feedback;
  Project project(".", &Feedback);

  auto Cycles = project.getCycles();

  UsedModulesDict UsableModules;

  for (auto& Cycle : Cycles) {
    for (auto& Module : Cycle) {
      if (UsableModules.find(Module) == UsableModules.end()) {
        for (auto& OtherModule : project.getModules()) {
          // Can always use the current module
          if (&OtherModule == Module) {
            UsableModules[Module].insert(Module);
          } else {
            // If the usage of this other module doesn't cause a cycle, then
            // we can use it.
            if (!OtherModule.getPathTo(Module)) {
              UsableModules[Module].insert(&OtherModule);
            }
          }
        }
      }
    }
  }

  writeCycleReport(Cycles, project, UsableModules);


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