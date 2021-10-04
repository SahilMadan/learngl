#ifndef LEARNGL_MODEL_HPP_
#define LEARNGL_MODEL_HPP_

#include <assimp/material.h>
#include <assimp/scene.h>
#include <assimp/texture.h>

#include <string>
#include <vector>
#include <unordered_set>

#include "mesh.hpp"
#include "shader_m.hpp"

class Model {
 public:
  Model(std::string path);
  void Draw(Shader& shader);
  const std::vector<Mesh>& Meshes() const;

 private:
  // Model data
  std::vector<Mesh> meshes_;
  std::string directory_;
  std::vector<Texture> loaded_textures_;

  void LoadModel(std::string path);
  void ProcessNode(aiNode* node, const aiScene* scene);
  Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

  std::vector<Texture> LoadMaterialTextures(aiMaterial* material,
                                            aiTextureType type,
                                            std::string type_name);
};

#endif
