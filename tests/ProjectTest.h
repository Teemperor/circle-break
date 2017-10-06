#include <Module.h>
#include <gtest/gtest.h>
#include <Project.h>
#include <DependencyTree.h>

TEST(SCRAMProject, Project1) {
  ProjectConfiguration Config;
  SCRAMProject project("SCRAMproject1", Config);

  ASSERT_EQ(project.getModules().size(), 3);


  for (auto& Module : project.getModules()) {
    if (Module.getPath() == "SCRAMproject1/DataFormats/Common/interface") {
      ASSERT_TRUE(Module.hasHeader(current_cwd() + "SCRAMproject1/DataFormats/Common/interface/DataFormatCommon1.h"));
      ASSERT_TRUE(Module.hasHeader(current_cwd() + "SCRAMproject1/DataFormats/Common/interface/DataFormatCommon2.h"));
      ASSERT_EQ(Module.getDependsOn().size(), 1);
      ASSERT_EQ(Module.getDependsOn().front()->getPath(), std::string("SCRAMproject1/FWCore/Common/interface"));

    } else if (Module.getPath() == "SCRAMproject1/FWCore/Common/interface") {
      ASSERT_TRUE(Module.hasHeader(current_cwd() + "SCRAMproject1/FWCore/Common/interface/FWCoreCommon1.h"));
      ASSERT_TRUE(Module.hasHeader(current_cwd() + "SCRAMproject1/FWCore/Common/interface/FWCoreCommon2.h"));
      ASSERT_EQ(Module.getDependsOn().size(), 1);
      ASSERT_EQ(Module.getDependsOn().front()->getPath(), std::string("SCRAMproject1/DataFormats/Common/interface"));

    } else if (Module.getPath() == "SCRAMproject1/FWCore/Framework/interface") {
      ASSERT_TRUE(Module.hasHeader(current_cwd() + "SCRAMproject1/FWCore/Framework/interface/FWCoreFramework1.h"));
      ASSERT_TRUE(Module.hasHeader(current_cwd() + "SCRAMproject1/FWCore/Framework/interface/FWCoreFramework2.h"));
      ASSERT_EQ(Module.getDependsOn().size(), 1);
      ASSERT_EQ(Module.getDependsOn().front()->getPath(), std::string("SCRAMproject1/FWCore/Common/interface"));

    } else {
      FAIL();
    }
  }

  ASSERT_EQ(project.getCycles().size(), 1);

}

TEST(LibraryProject, Project1) {
  ProjectConfiguration Config;
  Config.addIncludePath(".");
  LibraryProject project("library1", Config);

  ASSERT_EQ(project.getModules().size(), 1);

  const Module& M = project.getModules().front();

  std::set<std::string> Headers;
  std::set<std::string> ExpectedHeaders = {
    current_cwd() + "library1/Header1.h",
    current_cwd() + "library1/Header2.h",
    current_cwd() + "library1/Header3.h",
    current_cwd() + "library1/utils/UtilHeader1.h",
    current_cwd() + "library1/utils/UtilHeader2.h"
  };

  for (auto &H : M.getHeaders()) {
    Headers.insert(H.getAbsPath().str());
  }

  if (Headers != ExpectedHeaders) {
    std::cerr << "Doesn't match expected headers, got:\n";
    for (auto& H : Headers)
      std::cerr << H << "\n";
  }
  ASSERT_TRUE(Headers == ExpectedHeaders);

  DependencyTree Tree(project, Config.getFeedback());

  std::vector<std::string> ExpectedHeaderOrder = {
      current_cwd() + "library1/utils/UtilHeader1.h",
      current_cwd() + "library1/Header1.h",
      current_cwd() + "library1/utils/UtilHeader2.h",
      current_cwd() + "library1/Header2.h",
      current_cwd() + "library1/Header3.h",
  };

  std::vector<std::string> HeaderOrder;
  for (auto& H : Tree.getHeaderOrder()) {
    HeaderOrder.push_back(H.getAbsPath().str());
  }

  if (HeaderOrder != ExpectedHeaderOrder) {
    std::cerr << "Doesn't match expected header order, got:\n";
    for (auto& H : HeaderOrder)
      std::cerr << H << "\n";

  }
  ASSERT_TRUE(HeaderOrder == ExpectedHeaderOrder);

}

TEST(LibraryProject, Project2) {
  ProjectConfiguration Config;
  Config.addIncludePath(".");
  LibraryProject project("library2", Config);

  ASSERT_EQ(project.getModules().size(), 1);

  const Module& M = project.getModules().front();

  std::set<std::string> Headers;
  std::set<std::string> ExpectedHeaders = {
      current_cwd() + "library2/Header1.h",
      current_cwd() + "library2/Header2.h",
      current_cwd() + "library2/Header3.h",
      current_cwd() + "library2/utils/UtilHeader1.h",
      current_cwd() + "library2/utils/UtilHeader2.h"
  };

  for (auto &H : M.getHeaders()) {
    Headers.insert(H.getAbsPath().str());
  }

  if (Headers != ExpectedHeaders) {
    std::cerr << "Doesn't match expected headers, got:\n";
    for (auto& H : Headers)
      std::cerr << H << "\n";
  }
  ASSERT_TRUE(Headers == ExpectedHeaders);

  DependencyTree Tree(project, Config.getFeedback());

  std::vector<std::string> ExpectedHeaderOrder = {
      current_cwd() + "library2/Header2.h",
      current_cwd() + "library2/Header1.h",
      current_cwd() + "library2/Header3.h",
      current_cwd() + "library2/utils/UtilHeader1.h",
      current_cwd() + "library2/utils/UtilHeader2.h",
  };

  std::vector<std::string> HeaderOrder;
  for (auto& H : Tree.getHeaderOrder()) {
    HeaderOrder.push_back(H.getAbsPath().str());
  }

  if (HeaderOrder != ExpectedHeaderOrder) {
    std::cerr << "Doesn't match expected header order, got:\n";
    for (auto& H : HeaderOrder)
      std::cerr << H << "\n";

  }
  ASSERT_TRUE(HeaderOrder == ExpectedHeaderOrder);

}
