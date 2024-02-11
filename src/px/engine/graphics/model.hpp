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
#include "px/memory/ref_counting.hpp"

namespace px {

  class Model : public RefCounting {
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

    static std::vector<Ref<Texture>> loadMaterialTextures(aiMaterial *mat, aiTextureType type, [[maybe_unused]] const std::string &path);
  };
} // px

#endif //PX_ENGINE_MODEL_HPP
