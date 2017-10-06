#include <Header.h>
#include <gtest/gtest.h>

TEST(Header, parse) {
  IncludePaths IncludePaths;
  IncludePaths.addPath("testincludes");

  Header header("H1.h", "/H1.h");
  header.parseLine("#include <iostream>", IncludePaths, 1);
  header.parseLine("// a comment", IncludePaths, 2);
  header.parseLine("#include \"File.h\"", IncludePaths, 3);
  header.parseLine("// a comment", IncludePaths, 4);
  header.parseLine("#include \"subdir/File.h\"", IncludePaths, 5);
  header.parseLine(" #include <regex>", IncludePaths, 6);
  header.parseLine("# include <vector>", IncludePaths, 7);
  header.parseLine(" # include  <locale>", IncludePaths, 8);

  ASSERT_EQ(header.getIncludedHeaders().size(), 6);
  ASSERT_EQ(header.getIncludedHeaders().at(0).getFile().str(), current_cwd() + "testincludes/iostream");
  ASSERT_EQ(header.getIncludedHeaders().at(0).getLineNumber(), 1);
  ASSERT_EQ(header.getIncludedHeaders().at(1).getFile().str(), current_cwd() + "testincludes/File.h");
  ASSERT_EQ(header.getIncludedHeaders().at(1).getLineNumber(), 3);
  ASSERT_EQ(header.getIncludedHeaders().at(2).getFile().str(), current_cwd() + "testincludes/subdir/File.h");
  ASSERT_EQ(header.getIncludedHeaders().at(2).getLineNumber(), 5);
  ASSERT_EQ(header.getIncludedHeaders().at(3).getFile().str(), current_cwd() + "testincludes/regex");
  ASSERT_EQ(header.getIncludedHeaders().at(3).getLineNumber(), 6);
  ASSERT_EQ(header.getIncludedHeaders().at(4).getFile().str(), current_cwd() + "testincludes/vector");
  ASSERT_EQ(header.getIncludedHeaders().at(4).getLineNumber(), 7);
  ASSERT_EQ(header.getIncludedHeaders().at(5).getFile().str(), current_cwd() + "testincludes/locale");
  ASSERT_EQ(header.getIncludedHeaders().at(5).getLineNumber(), 8);
}
