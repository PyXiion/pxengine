// Copyright (c) 2023.

//
// Created by pyxiion on 22.11.23.
//

#include "model.hpp"
#include <stack>
#include <easylogging++.h>

namespace px {
  void Model::loadFromFile(const std::string &path) {
    CLOG(INFO, "PXEngine") << "Loading new model from " << path;
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      throw std::runtime_error("TODO: an adequate exception"); // TODO
    }
    processNodes(scene->mRootNode, scene);
    CLOG(INFO, "PXEngine") << "The model have been loaded successfully";
  }

  void Model::loadFromStream(std::istream &stream) {
    CLOG(INFO, "PXEngine") << "Loading new model from stream (" << &stream << ")";

    std::vector<char> buffer {std::istreambuf_iterator<char>(stream),
                              std::istreambuf_iterator<char>()};

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFileFromMemory(buffer.data(), buffer.size(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      throw std::runtime_error("TODO: an adequate exception"); // TODO
    }
    processNodes(scene->mRootNode, scene);
    CLOG(INFO, "PXEngine") << "The model have been loaded successfully";
  }

  void Model::draw(const RenderStates &renderStates) const {
    for (auto &mesh : m_meshes) {
      mesh.submit(renderStates);
    }
  }

  void Model::processNodes(aiNode *rootNode, const aiScene *scene) {
    size_t processedNodesCount = 0;
    size_t processedMeshesCount = 0;

    std::stack<aiNode *> nodes;
    nodes.push(rootNode);

    while (not nodes.empty()) {
      auto node = nodes.top();
      nodes.pop();
      processedNodesCount += 1;

      // process all the node's meshes
      for (uint i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
        processedMeshesCount += 1;
      }
      // process children
      for (uint i = 0; i < node->mNumChildren; i++) {
        nodes.push(node->mChildren[i]);
      }
    }
    CLOG(INFO, "PXEngine") << "\tNodes: "  << processedNodesCount;
    CLOG(INFO, "PXEngine") << "\tMeshes: " << processedMeshesCount;
  }

  Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<IndexType> indices;
    std::vector<TexturePtr> textures;

    bool hasTexCoords = mesh->mTextureCoords[0];

    CLOG(INFO, "PXEngine") << "\tFound a mesh with " << mesh->mNumVertices << " vertices and " << mesh->mNumFaces << " faces";

    for (uint i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex{};

      auto v = &mesh->mVertices[i];
      vertex.position = {v->x, v->y, v->z};

      auto n = &mesh->mNormals[i];
      vertex.normal = {n->x, n->y, n->z};

      if (hasTexCoords) {
        auto t = &mesh->mTextureCoords[0][i];
        vertex.texCoords = {t->x, t->y};
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

  std::vector<TexturePtr> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, [[maybe_unused]] const std::string &path) {
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

  const std::vector<Mesh> &Model::getMeshes() {
    return m_meshes;
  }

  std::vector<std::string> resources::Traits<Model>::extensions {
    ".obj"
  };

  Resource<Model> resources::Traits<Model>::load(ResourceManager &rm, std::istream &stream) {
    auto model = makeModel();
    model->loadFromStream(stream);
    return model;
  }
} // pc