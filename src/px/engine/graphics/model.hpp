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
#include "mesh.hpp"
#include "shader.hpp"
#include "render_states.hpp"

namespace px {

  class Model {
  public:
    void loadFromFile(const std::string &path);

    void draw(const RenderStates &renderStates) const;

  private:
    std::vector<Mesh> m_meshes;

    void processNodes(aiNode *rootNode, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    static std::vector<TexturePtr> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &path);
  };

  typedef std::shared_ptr<Model> ModelPtr;
  template <class ...TArgs>
  inline static ModelPtr makeModel(TArgs ...args) { return std::make_shared<Model>(std::forward<TArgs>(args)...); }

} // px

#endif //PX_ENGINE_MODEL_HPP
