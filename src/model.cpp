#include "model.hpp"

#include <assimp/material.h>
#include <assimp/postprocess.h>

#include <assimp/Importer.hpp>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_include.hpp"

unsigned int TextureFromFile(const char* path, const std::string& directory);

Model::Model(std::string path) { LoadModel(path); }

void Model::Draw(Shader& shader) {
  for (unsigned int i = 0; i < meshes_.size(); i++) {
    meshes_[i].Draw(shader);
  }
}

void Model::LoadModel(std::string path) {
  Assimp::Importer import;
  const aiScene* scene =
      import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    std::cerr << "Error::Assimp::" << import.GetErrorString() << std::endl;
    return;
  }
  directory_ = path.substr(0, path.find_last_of('/'));

  ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes_.push_back(ProcessMesh(mesh, scene));
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    ProcessNode(node->mChildren[i], scene);
  }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    // Process vertex positions, normals and texture coordinates
    Vertex vertex;
    glm::vec3 vector;
    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;
    vertex.position = vector;

    vector.x = mesh->mNormals[i].x;
    vector.y = mesh->mNormals[i].y;
    vector.z = mesh->mNormals[i].z;
    vertex.normal = vector;

    if (mesh->mTextureCoords[0]) {
      glm::vec2 vec;
      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;
      vertex.tex_coords = vec;
    } else {
      vertex.tex_coords = glm::vec2(0.0f, 0.0f);
    }
    vertices.push_back(vertex);
  }

  // Process indices

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  // Process material
  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
  std::vector<Texture> diffuse_maps =
      LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
  textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
  std::vector<Texture> specular_maps = LoadMaterialTextures(
      material, aiTextureType_SPECULAR, "texture_specular");
  textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

  return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* material,
                                                 aiTextureType type,
                                                 std::string type_name) {
  std::vector<Texture> textures;
  for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
    aiString str;
    material->GetTexture(type, i, &str);
    bool skip = false;
    for (unsigned int j = 0; j < loaded_textures_.size(); j++) {
      if (std::strcmp(loaded_textures_[j].path.data(), str.C_Str()) == 0) {
        textures.push_back(loaded_textures_[j]);
        skip = true;
        break;
      }
    }
    if (!skip) {
      Texture texture;
      texture.id = TextureFromFile(str.C_Str(), directory_);
      texture.type = type_name;
      texture.path = str.C_Str();
      textures.push_back(texture);
      loaded_textures_.push_back(texture);
    }
  }
  return textures;
}

unsigned int TextureFromFile(const char* path, const std::string& directory
                             ) {
  std::string filename = std::string(path);
  filename = directory + '/' + filename;

  unsigned int texture_id;
  glGenTextures(1, &texture_id);

  int width;
  int height;
  int component_count;
  unsigned char* data =
      stbi_load(filename.c_str(), &width, &height, &component_count, 0);
  if (data) {
    GLenum format;
    switch (component_count) {
      case 1:
        format = GL_RED;
        break;
      case 3:
        format = GL_RGB;
        break;
      case 4:
        format = GL_RGBA;
        break;
      default:
        std::cerr << "Unsupported texture at path: " << path << std::endl;
        stbi_image_free(data);
        return texture_id;
    }

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 00, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  } else {
    std::cerr << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }
  return texture_id;
}
