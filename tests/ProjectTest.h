#include <Module.h>
#include <gtest/gtest.h>
#include <Project.h>

TEST(Project, Project1) {
  Project project("project1");

  ASSERT_EQ(project.getModules().size(), 3);


  for (auto& Module : project.getModules()) {
    if (Module.getPath() == "project1/DataFormats/Common/interface") {
      ASSERT_TRUE(Module.hasHeader("project1/DataFormats/Common/interface/DataFormatCommon1.h"));
      ASSERT_TRUE(Module.hasHeader("project1/DataFormats/Common/interface/DataFormatCommon2.h"));
      ASSERT_EQ(Module.getDependsOn().size(), 1);
      ASSERT_EQ(Module.getDependsOn().front()->getPath(), std::string("project1/FWCore/Common/interface"));

    } else if (Module.getPath() == "project1/FWCore/Common/interface") {
      ASSERT_TRUE(Module.hasHeader("project1/FWCore/Common/interface/FWCoreCommon1.h"));
      ASSERT_TRUE(Module.hasHeader("project1/FWCore/Common/interface/FWCoreCommon2.h"));
      ASSERT_EQ(Module.getDependsOn().size(), 1);
      ASSERT_EQ(Module.getDependsOn().front()->getPath(), std::string("project1/DataFormats/Common/interface"));

    } else if (Module.getPath() == "project1/FWCore/Framework/interface") {
      ASSERT_TRUE(Module.hasHeader("project1/FWCore/Framework/interface/FWCoreFramework1.h"));
      ASSERT_TRUE(Module.hasHeader("project1/FWCore/Framework/interface/FWCoreFramework2.h"));
      ASSERT_EQ(Module.getDependsOn().size(), 1);
      ASSERT_EQ(Module.getDependsOn().front()->getPath(), std::string("project1/FWCore/Common/interface"));

    } else {
      FAIL();
    }
  }

  ASSERT_EQ(project.getCycles().size(), 1);

}
