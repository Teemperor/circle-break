
#include "gtest/gtest.h"

#include "StringCacheTest.h"
#include "HeaderIncludeTest.h"
#include "HeaderTest.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}