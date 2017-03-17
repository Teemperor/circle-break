#include <StringCache.h>
#include <gtest/gtest.h>

TEST(StringCache, getID) {
  auto id = StringCache::get()["foo"];
  ASSERT_EQ(id, StringCache::get()["foo"]);
  ASSERT_NE(id, StringCache::get()["foo1"]);
}

TEST(StringCache, getString) {
  auto id = StringCache::get()["foo"];
  ASSERT_EQ(StringCache::get()[id], "foo");

  id = StringCache::get()["foo1"];
  ASSERT_EQ(StringCache::get()[id], "foo1");

  // just check that we don't just return the latest
  auto id1 = StringCache::get()["foo"];
  ASSERT_EQ(StringCache::get()[id], "foo1");
  ASSERT_EQ(StringCache::get()[id1], "foo");
}