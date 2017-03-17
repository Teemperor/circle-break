#include <Module.h>
#include <gtest/gtest.h>

TEST(Module, Module1) {
  IncludePaths IncludePaths;

  Module module;

  module.parseDirectory("module1", IncludePaths);

  ASSERT_EQ(module.getHeaders().size(), 3);

  for (auto& Header : module.getHeaders()) {
    if (Header.getPath() == "module1/H1.h") {
      ASSERT_EQ(Header.getIncludedHeaders().size(), 1);
      ASSERT_EQ(Header.getIncludedHeaders().front().getFile().str(), "module1/H2.h");
    } else if (Header.getPath() == "module1/H2.h") {
      ASSERT_EQ(Header.getIncludedHeaders().size(), 1);
      ASSERT_EQ(Header.getIncludedHeaders().front().getFile().str(), "module1/H1.h");
    } else if (Header.getPath() == "module1/H3.h") {
      ASSERT_EQ(Header.getIncludedHeaders().size(), 1);
      ASSERT_EQ(Header.getIncludedHeaders().front().getFile().str(), "module1/H1.h");
    } else {
      FAIL();
    }
  }


}
