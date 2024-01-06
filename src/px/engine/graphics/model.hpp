// Copyright (c) 2023.

//
// Created by pyxiion on 22.11.23.
//

#ifndef PX_ENGINE_MODEL_HPP
#define PX_ENGINE_MODEL_HPP

#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "px/engine/resources/resource_traits.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "render_states.hpp"

namespace px {

  class Model {
  public:
    Model() = default;
    explicit Model(const std::string &path) { loadFromFile(path); }

    void loadFromFile(const std::string &path);
    void loadFromStream(std::istream &stream);

    void draw(const RenderStates &renderStates) const;

    const std::vector<Mesh> &getMeshes();

  private:
    std::vector<Mesh> m_meshes;

    void processNodes(aiNode *rootNode, const aiScene *scene);
    static Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    static std::vector<TexturePtr> loadMaterialTextures(aiMaterial *mat, aiTextureType type, [[maybe_unused]] const std::string &path);
  };

  typedef std::shared_ptr<Model> ModelPtr;
  template <class ...TArgs>
  static ModelPtr makeModel(TArgs &&...args) { return std::make_shared<Model>(std::forward<TArgs>(args)...); }

  namespace resources {
    template<>
    struct Traits<Model> {
      static std::vector<std::string> extensions;

      static Resource<Model> load(ResourceManager &rm, std::istream &stream);
    };
  }
} // px

#endif //PX_ENGINE_MODEL_HPP
