
#ifndef CIRCLE_BREAK_DEPENDENCYTREE_H
#define CIRCLE_BREAK_DEPENDENCYTREE_H

#include <Project.h>
#include <set>

class DependencyTree {

  Project *P = nullptr;
  ProjectFeedback* Feedback = nullptr;
  std::vector<Header> HeadersToRemove;
  std::set<Header> HandledHeaders;
  std::set<Header> UnhandledHeaders;
  std::map<Header, std::vector<Header> > IsIncludedBy;
  std::map<Header, std::set<Header> > UnhandledIncludes;

  bool CreatedHeaderOrder = false;
  std::vector<Header> HeaderOrder;

  bool indirectlyTargetIncludedBy(const Header& Target, const Header& Includer) {
    for (auto &I : IsIncludedBy[Target]) {
      if (I.getAbsPath() == Includer.getAbsPath()) {
        return true;
      }
      return indirectlyTargetIncludedBy(I, Includer);
    }
    return false;
  }

  std::vector<Header> getIncludeChain(const Header& To, const Header& From, std::set<Header>& AlreadyChecked) {
    std::vector<Header> Result;
    for (auto &Includer : IsIncludedBy[To]) {
      if (AlreadyChecked.find(Includer) != AlreadyChecked.end())
        continue;
      AlreadyChecked.insert(Includer);
      if (Includer.getAbsPath() == From.getAbsPath()) {
        return {Includer};
      }

      auto NewChain = getIncludeChain(Includer, From, AlreadyChecked);
      if (NewChain.empty())
        continue;
      Result.push_back(Includer);
      for (auto& H : NewChain)
        Result.push_back(H);
    }
    return Result;
  }

  void fixHeaderOrderCycle() {
    const Header* BestHeader = nullptr;
    std::size_t MinVal = std::numeric_limits<std::size_t>::max();
    for (auto &H : UnhandledHeaders) {
      std::set<Header> AlreadyChecked;
      auto Cycle = getIncludeChain(H, H, AlreadyChecked);
      if (!Cycle.empty()) {
        if (Cycle.size() < MinVal) {
          MinVal = Cycle.size();
          BestHeader = &H;
        }
      }
    }

    const Header& H = *BestHeader;
    std::cerr << "Had to break cycle for " << H.getPath().str() << ".\n";
    std::set<Header> AlreadyChecked;
    auto Cycle = getIncludeChain(H, H, AlreadyChecked);
    /*std::cerr << "Inclusion cycle: ";
    for (std::size_t i = 0; i < Cycle.size(); ++i) {
      auto &CycleH = Cycle[i];
      std::cerr << CycleH.getPath().str();
      if (i + 1 != Cycle.size())
        std::cerr << " -> ";
    }
    std::cerr << std::endl;*/

    UnhandledIncludes[H].clear();
    addHeaderToHeaderOrder(H);
    deleteQueuedHeaders();
  }

  void addHeaderToHeaderOrder(const Header& H) {
    HandledHeaders.insert(H);
    HeadersToRemove.push_back(H);
    HeaderOrder.push_back(H);

    for (auto& Includer : IsIncludedBy[H]) {
      UnhandledIncludes[Includer].erase(H);
    }
  }

  void deleteQueuedHeaders() {
    for (auto &H : HeadersToRemove) {
      auto removed = UnhandledHeaders.erase(H);
      assert(removed != 0);
    }
    HeadersToRemove.clear();
  }

  void createHeaderOrder() {
    assert(!CreatedHeaderOrder);

    for (auto &M : P->getModules()) {
      for (auto &H : M.getHeaders()) {
        UnhandledHeaders.insert(H);
        for (auto &I : H.getIncludedHeaders()) {
          if (!I.knowTarget())
            continue;
          if (I.getHeader().getAbsPath() == H.getAbsPath()) {
            std::cerr << "Warning: Header includes itself " << I.getHeader().getAbsPath().str() << std::endl;
            continue;
          }

          UnhandledIncludes[H].insert(I.getHeader());
          IsIncludedBy[I.getHeader()].push_back(H);
        }
      }
    }

    while (true) {
      bool DoingProgress = false;
      for (auto &H : UnhandledHeaders) {
        if (UnhandledIncludes[H].empty()) {
          DoingProgress = true;
          addHeaderToHeaderOrder(H);
        }
      }
      deleteQueuedHeaders();

      if (!DoingProgress) {
        if (!UnhandledHeaders.empty()) {
          fixHeaderOrderCycle();
          continue;
        }
        break;
      }
    }
  }

public:
  DependencyTree() {}
  DependencyTree(Project &P, ProjectFeedback& F) : P(&P), Feedback(&F) {
  }

  const std::vector<Header>& getHeaderOrder() {
    if (!CreatedHeaderOrder)
      createHeaderOrder();
    return HeaderOrder;
  }
};

#endif //CIRCLE_BREAK_DEPENDENCYTREE_H
