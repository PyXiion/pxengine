// Copyright (c) 2023.

//
// Created by pyxiion on 22.11.23.
//

#include "model.hpp"
#include <stack>

namespace px {
  void Model::loadFromFile(const std::string &path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      throw std::runtime_error("TODO: an adequate exception");
    }
    processNodes(scene->mRootNode, scene);


  }

  void Model::draw(const RenderStates &renderStates) const {
    for (auto &mesh : m_meshes) {
      mesh.apply(0); // TODO
      bgfx::submit(renderStates.viewId, renderStates.shaderPtr->get());
    }
  }

  void Model::processNodes(aiNode *rootNode, const aiScene *scene) {
    std::stack<aiNode *> nodes;
    nodes.push(rootNode);

    while (not nodes.empty()) {
      auto node = nodes.top();
      nodes.pop();

      // process all the node's meshes
      for (uint i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
      }
      // process children
      for (uint i = 0; i < node->mNumChildren; i++) {
        nodes.push(node->mChildren[i]);
      }
    }
  }

  Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<IndexType> indices;
    std::vector<TexturePtr> textures;

    bool hasTexCoords = mesh->mTextureCoords[0];

    printf("Загружаю меш с %d вертексами\n", mesh->mNumVertices);

    for (uint i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex{};

      auto v = &mesh->mVertices[i];
      vertex.position.x = v->x;
      vertex.position.y = v->y;
      vertex.position.z = v->z;

      auto n = &mesh->mNormals[i];
      vertex.normal.x = n->x;
      vertex.normal.y = n->y;
      vertex.normal.z = n->z;

      if (hasTexCoords) {
        auto t = &mesh->mTextureCoords[0][i];
        vertex.texCoords.x = t->x;
        vertex.texCoords.y = t->y;
      } else {
        vertex.texCoords = {0, 0};
      }

      vertices.push_back(vertex);
    }

    for (uint i = 0; i < mesh->mNumFaces; i++) {
      aiFace *face = &mesh->mFaces[i];
      for (uint j = 0; j < face->mNumIndices; j++) {
        indices.push_back(face->mIndices[j]);
      }
    }

    if (mesh->mMaterialIndex >= 0) {
      aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

      std::vector<TexturePtr> diffuseMaps = loadMaterialTextures(material,
                                                                 aiTextureType_DIFFUSE,
                                                                 "texture_diffuse");
      textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

      std::vector<TexturePtr> specularMaps = loadMaterialTextures(material,
                                                                  aiTextureType_SPECULAR,
                                                                 "texture_specular");
      textures.insert(specularMaps.end(), specularMaps.begin(), specularMaps.end());
    }

    return {std::move(vertices), std::move(indices), std::move(textures)};
  }

  std::vector<TexturePtr> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &path) {
    std::vector<TexturePtr> textures;
    for (uint i = 0; i < mat->GetTextureCount(type); i++) {
      aiString str;
      mat->GetTexture(type, i, &str);

      TexturePtr texture = makeTexture();
      texture->loadFromFile(str.C_Str());
      textures.push_back(std::move(texture));
    }

    return textures;
  }
} // pc