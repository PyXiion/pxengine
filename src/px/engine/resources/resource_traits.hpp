// Copyright (c) 2023.

//
// Created by pyxiion on 26.12.23.
//

#ifndef PX_ENGINE_RESOURCE_TRAITS_HPP
#define PX_ENGINE_RESOURCE_TRAITS_HPP
#include <vector>
#include <string>
#include <fstream>
#include <memory>

namespace px {

  class ResourceManager;

  template<class T>
  using Resource = std::shared_ptr<T>;

  template<class T, class ...TArgs>
  Resource<T> makeResource(TArgs &&...args) {
    return std::make_shared<T>(std::forward<TArgs>(args)...);
  }

  extern std::vector<std::string> parseId(const std::string &id);

  namespace resources {
    std::string getDefaultPath(const std::string &root, const std::string &id, const std::vector<std::string> &extensions);

    // hack to print the type name
    template<class U> struct TypeName { static inline constexpr bool value = false; };

    template<class T>
    struct Traits;

  } // resources


  namespace priv {
    template<class T>
    struct ResourceTraitsDataImpl : std::false_type {};

    template<class T>
    struct ResourceTraitsDataImpl<resources::Traits<T>> : std::true_type {
      typedef Resource<T> ResourceType;
    };

    template<class T>
    inline constexpr bool IsResourceTraits = ResourceTraitsDataImpl<T>::value;

    template<class T>
    using ResourceType = typename ResourceTraitsDataImpl<T>::ResourceType;
  } // priv

  template<class T>
  concept LoadableResourceTraits = requires {
    { T::load(std::declval<std::istream&>()) } -> std::convertible_to<priv::ResourceType<T>>;
  };

  template<class T>
  concept LoadableWithManagerResourceTraits = requires {
    { T::load(std::declval<ResourceManager&>(), std::declval<std::istream&>()) } -> std::convertible_to<priv::ResourceType<T>>;
  };

  template<class T>
  concept SavableResourceTraits = LoadableResourceTraits<T> and requires (std::ofstream &ofs, priv::ResourceType<T> resource) {
    { T::save(std::ref(ofs), std::cref(resource)) };
  };

  template<class T>
  concept HasResourceTraitGetPath = requires {
    { T::getPath(std::declval<std::string>(), std::declval<std::string>()) } -> std::convertible_to<std::string>;
  };

  template<class T>
  concept HasResourceTraitExtensions = requires {
    { T::extensions } -> std::convertible_to<std::vector<std::string>&>;
  };

  // helper functions
  namespace resources {
    static thread_local std::string currentResourceId;

    void setCurrentResourceId(const std::string &value);

    const std::string &getCurrentResourceId();

    template<class Traits>
    std::string getPath(const std::string &root, const std::string &resourceId) {
      if constexpr (HasResourceTraitGetPath<Traits>) {
        return Traits::getPath(root, resourceId);
      } else if constexpr (HasResourceTraitExtensions<Traits>) {
        return getDefaultPath(root, resourceId, Traits::extensions);
      } else {
        static_assert(TypeName<Traits>::value, "Failed to getPath for the type. Declare 'static std::string getPath(std::string resourceId)' or a list of extensions 'static std::vector<std::string> extensions'");
      }
    }

    template<class Traits>
    auto load(ResourceManager &rm, std::istream &istream) {
      if constexpr (LoadableResourceTraits<Traits>) {
        return Traits::load(istream);
      } else if constexpr (LoadableWithManagerResourceTraits<Traits>){
        return Traits::load(rm, istream);
      } else {
        static_assert(TypeName<Traits>::value);
      }
    }

    // text files
    template<>
    struct Traits<std::string> {
      static std::vector<std::string> extensions;

      static Resource<std::string> load(std::istream &is);
    };

    // binary files
    template<>
    struct Traits<std::vector<char>> {
      static std::vector<std::string> extensions;

      static Resource<std::vector<char>> load(std::istream &is);
    };
  }
} // px

#endif //PX_ENGINE_RESOURCE_TRAITS_HPP