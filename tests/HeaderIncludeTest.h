#include <HeaderInclude.h>
#include <gtest/gtest.h>

TEST(HeaderInclude, parse) {
  IncludePaths IncludePaths;
  IncludePaths.addPath("testincludes");

  HeaderInclude include;
  EXPECT_TRUE(include.parse("#include <iostream>", IncludePaths));
  EXPECT_EQ(include.getFile().str(), "testincludes/iostream");
  EXPECT_TRUE(include.parse(" #include <string>", IncludePaths));
  EXPECT_EQ(include.getFile().str(), "testincludes/string");
  EXPECT_TRUE(include.parse("   #include <locale>", IncludePaths));
  EXPECT_EQ(include.getFile().str(), "testincludes/locale");
  EXPECT_TRUE(include.parse("#include <regex> // mombo", IncludePaths));
  EXPECT_EQ(include.getFile().str(), "testincludes/regex");
  EXPECT_TRUE(include.parse("  #include <vector> // mambo", IncludePaths));
  EXPECT_EQ(include.getFile().str(), "testincludes/vector");

  // nonexisting file
  EXPECT_FALSE(include.parse("#include <nonexistingfile>", IncludePaths));

  // malformed includes
  EXPECT_FALSE(include.parse("#include <iostream", IncludePaths));
  EXPECT_FALSE(include.parse("#include iostream>", IncludePaths));
  EXPECT_FALSE(include.parse("#inclue iostream>", IncludePaths));
  EXPECT_FALSE(include.parse("inclue iostream>", IncludePaths));
  EXPECT_FALSE(include.parse("//#include <iostream>", IncludePaths));
  EXPECT_FALSE(include.parse("* #include <iostream>", IncludePaths));
}
