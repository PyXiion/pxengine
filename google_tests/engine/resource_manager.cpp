// Copyright (c) 2023.

//
// Created by pyxiion on 30.12.23.
//

#include <gtest/gtest.h>
#include <fmt/format.h>
#include <filesystem>
#include <ranges>
#include <px/engine/resources/resource_manager.hpp>

namespace fs = std::filesystem;
namespace views = std::views;

namespace {
  struct SomeResource {
    std::string value;
  };

  struct SomeResourceWithExtensions : public SomeResource {};
}

namespace px::resources {
  template<>
  struct Traits<SomeResource> {
    static std::string getPath(const std::string &resourceId) {
      fs::path path;
      auto parsed = parseId(resourceId);
      auto last = parsed.size() - 1;

      for (const auto &i : parsed | views::take(last)) {
        path /= i;
      }

      path /= "folder_before_file";
      path /= parsed[last];

      return path;
    }
  };

  template<>
  struct Traits<SomeResourceWithExtensions> {
    inline static std::vector<std::string> extensions = {
        ".txt", ".text"
    };
    static Resource<SomeResourceWithExtensions> load(std::istream &is){
      auto resource = std::make_shared<SomeResourceWithExtensions>();
      resource->value = std::string{std::istreambuf_iterator<char>(is),
                                    std::istreambuf_iterator<char>()};
      return resource;
    };
  };
}

static std::string someText = "Some resource";
static std::string someOtherText = "Other resource";

class ResourceManagerTest : public testing::Test {
protected:
  static void SetUpTestSuite() {
    m_previousPath = fs::current_path();

    // set the cwd to a temporary
    auto temp = fs::temp_directory_path();
    temp /= std::tmpnam(nullptr);

    fs::create_directory(temp);
    fs::current_path(temp);

    // create some resources
    fs::create_directories("path/to/");
    fs::create_directories("path/to/other");

    std::ofstream ofs("path/to/resource.txt");
    ofs << someText;

    ofs = std::ofstream("path/to/other/resource.text");
    ofs << someOtherText;
  }

  static void TearDownTestSuite() {
    // remove the temporary
    fs::remove_all(fs::current_path());

    // set the cwd back
    fs::current_path(m_previousPath);
  }

  inline static px::ResourceManager resourceManager{"./"};

private:
  inline static fs::path m_previousPath;
};

// testing resources::getPath
TEST_F(ResourceManagerTest, GetPathTest) {
  using namespace px::resources;

  auto path = getPath<Traits<SomeResource>>("path.to.resource");
  ASSERT_EQ(path, "path/to/folder_before_file/resource");

  path = getPath<Traits<SomeResource>>("path.to.other.resource");
  ASSERT_EQ(path, "path/to/other/folder_before_file/resource");

  path = getPath<Traits<SomeResourceWithExtensions>>("path.to.resource");
  ASSERT_EQ(path, "path/to/resource.txt");

  path = getPath<Traits<SomeResourceWithExtensions>>("path.to.other.resource");
  ASSERT_EQ(path, "path/to/other/resource.text");

  // check non-exist files
  ASSERT_THROW(getPath<Traits<SomeResourceWithExtensions>>("path.to.nonexistent.resource"), std::runtime_error);
}

// test resource getting
TEST_F(ResourceManagerTest, LoadTest) {
  // get with non-loaded resource
  ASSERT_THROW(resourceManager.get<SomeResourceWithExtensions>("path.to.resource", false), std::runtime_error);
  ASSERT_THROW(resourceManager.get<SomeResourceWithExtensions>("path.to.other.resource", false), std::runtime_error);

  // load resource
  resourceManager.load<SomeResourceWithExtensions>("path.to.resource");
  auto resource = resourceManager.get<SomeResourceWithExtensions>("path.to.resource", false);
  ASSERT_EQ(resource->value, someText);

  // load if it doesn't exist
  resource = resourceManager.get<SomeResourceWithExtensions>("path.to.other.resource", true);
  ASSERT_EQ(resource->value, someOtherText);

  ASSERT_THROW(resourceManager.get<SomeResourceWithExtensions>("path.to.nonexistent.resource"), std::runtime_error);
}