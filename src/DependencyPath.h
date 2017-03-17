#ifndef CYCLEDETECT_PATH_H
#define CYCLEDETECT_PATH_H

#include <unordered_set>
#include <vector>

class Module;

class DependencyPath {

  std::vector<const Module*> Nodes;

public:
  DependencyPath() {
  }
  DependencyPath(const Module* M) : Nodes({M}) {
  }

  bool empty() const {
    return Nodes.empty();
  }

  const Module* back() const {
    return Nodes.back();
  }

  operator bool() const {
    return !empty();
  }

  std::size_t length() const {
    return Nodes.size();
  }

  void add(const Module* M) { Nodes.push_back(M); }

  std::vector<const Module*>::const_iterator begin() const {
    return Nodes.begin();
  }

  bool similar(const DependencyPath& Other) {
    std::unordered_set<const Module *> ThisModules;
    std::unordered_set<const Module *> OtherModules;

    ThisModules.insert(Nodes.begin(), Nodes.end());
    OtherModules.insert(Other.Nodes.begin(), Other.Nodes.end());

    return ThisModules == OtherModules;
  }

  std::vector<const Module*>::const_iterator end() const {
    return Nodes.end();
  }

};


#endif //CYCLEDETECT_PATH_H
