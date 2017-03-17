#include <Header.h>
#include <gtest/gtest.h>

TEST(Header, parse) {
  IncludePaths IncludePaths;
  IncludePaths.addPath("testincludes");

  Header header("H1.h");
  header.parseLine("#include <iostream>", IncludePaths);
  header.parseLine("// a comment", IncludePaths);
  header.parseLine("#include \"File.h\"", IncludePaths);
  header.parseLine("// a comment", IncludePaths);
  header.parseLine("#include \"subdir/File.h\"", IncludePaths);

  ASSERT_EQ(header.getIncludedHeaders().size(), 3);
  ASSERT_EQ(header.getIncludedHeaders().at(0).getFile().str(), "testincludes/iostream");
  ASSERT_EQ(header.getIncludedHeaders().at(0).getLineNumber(), 1);
  ASSERT_EQ(header.getIncludedHeaders().at(1).getFile().str(), "testincludes/File.h");
  ASSERT_EQ(header.getIncludedHeaders().at(1).getLineNumber(), 3);
  ASSERT_EQ(header.getIncludedHeaders().at(2).getFile().str(), "testincludes/subdir/File.h");
  ASSERT_EQ(header.getIncludedHeaders().at(2).getLineNumber(), 5);
}
