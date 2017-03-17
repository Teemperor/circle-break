#include <HeaderInclude.h>
#include <gtest/gtest.h>

TEST(HeaderInclude, parse) {
  IncludePaths IncludePaths;
  HeaderInclude include;
  EXPECT_TRUE(include.parse("#include <iostream>", IncludePaths));
  EXPECT_EQ(include.getFile().str(), "iostream");
  EXPECT_TRUE(include.parse(" #include <string>", IncludePaths));
  EXPECT_EQ(include.getFile().str(), "string");
  EXPECT_TRUE(include.parse("   #include <locale>", IncludePaths));
  EXPECT_EQ(include.getFile().str(), "locale");
  EXPECT_TRUE(include.parse("#include <regex> // mombo", IncludePaths));
  EXPECT_EQ(include.getFile().str(), "regex");
  EXPECT_TRUE(include.parse("  #include <vector> // mambo", IncludePaths));
  EXPECT_EQ(include.getFile().str(), "vector");

  EXPECT_FALSE(include.parse("#include <nonexistingfile>", IncludePaths));

  EXPECT_FALSE(include.parse("#include <iostream", IncludePaths));
  EXPECT_FALSE(include.parse("#include iostream>", IncludePaths));
  EXPECT_FALSE(include.parse("#inclue iostream>", IncludePaths));
  EXPECT_FALSE(include.parse("inclue iostream>", IncludePaths));
  EXPECT_FALSE(include.parse("//#include <iostream>", IncludePaths));
  EXPECT_FALSE(include.parse("* #include <iostream>", IncludePaths));
}
