#include "DependencyPath.h"

bool DependencyPath::similar(const DependencyPath &Other) {
  std::unordered_set<const Module *> ThisModules;
  std::unordered_set<const Module *> OtherModules;

  ThisModules.insert(Nodes.begin(), Nodes.end());
  OtherModules.insert(Other.Nodes.begin(), Other.Nodes.end());

  return ThisModules == OtherModules;
}
