#include <Module.h>
#include <gtest/gtest.h>
#include <ProjectFeedback.h>

TEST(Module, Module1) {
  IncludePaths IncludePaths;

  Module module;

  NoFeedback F;
  module.parseDirectory("module1", IncludePaths, F);

  ASSERT_EQ(module.getHeaders().size(), 3);

  for (auto& Header : module.getHeaders()) {
    if (Header.getAbsPath().str() == current_cwd() + "module1/H1.h") {
      ASSERT_EQ(Header.getIncludedHeaders().size(), 1);
      ASSERT_EQ(Header.getIncludedHeaders().front().getFile().str(), current_cwd() + "module1/H2.h");
    } else if (Header.getAbsPath().str() == current_cwd() + "module1/H2.h") {
      ASSERT_EQ(Header.getIncludedHeaders().size(), 1);
      ASSERT_EQ(Header.getIncludedHeaders().front().getFile().str(), current_cwd() + "module1/H1.h");
    } else if (Header.getAbsPath().str() == current_cwd() + "module1/H3.h") {
      ASSERT_EQ(Header.getIncludedHeaders().size(), 1);
      ASSERT_EQ(Header.getIncludedHeaders().front().getFile().str(), current_cwd() + "module1/H1.h");
    } else {
      FAIL();
    }
  }
}

TEST(Module, CycleDetection) {
  std::vector<Module> Modules = {
      Module("H0.h"),
      Module("H1.h"),
      Module("H2.h"),
      Module("H3.h"),
      Module("H4.h"),
      Module("H5.h")
  };

  // connect one module to the one with the next higher index
  Modules[0].addDependency(&Modules[1]);
  Modules[1].addDependency(&Modules[2]);
  Modules[2].addDependency(&Modules[3]);
  Modules[3].addDependency(&Modules[4]);
  Modules[4].addDependency(&Modules[5]);

  // shortcuts in the graph
  Modules[0].addDependency(&Modules[2]);
  Modules[0].addDependency(&Modules[3]);
  Modules[0].addDependency(&Modules[4]);
  Modules[1].addDependency(&Modules[3]);

  ASSERT_EQ(Modules[0].getPathTo(&Modules[5]).length(), 3);
  ASSERT_EQ(Modules[1].getPathTo(&Modules[5]).length(), 4);
  ASSERT_EQ(Modules[4].getPathTo(&Modules[5]).length(), 2);

  ASSERT_EQ(Modules[5].getPathTo(&Modules[4]).length(), 0);
  ASSERT_EQ(Modules[4].getPathTo(&Modules[3]).length(), 0);
  ASSERT_EQ(Modules[3].getPathTo(&Modules[2]).length(), 0);
  ASSERT_EQ(Modules[2].getPathTo(&Modules[1]).length(), 0);
  ASSERT_EQ(Modules[1].getPathTo(&Modules[0]).length(), 0);
}
