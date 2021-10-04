#ifndef LEARNGL_MESH_HPP_
#define LEARNGL_MESH_HPP_

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "shader_m.hpp"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 tex_coords;
};

struct Texture {
  unsigned int id;
  std::string type;
  // Store the path of the texture to compare with other textures
  std::string path;
};

class Mesh {
 public:
  // Mesh data
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;
  unsigned int vao() const;

  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
       std::vector<Texture> textures);
  void Draw(Shader &shader);

 private:
  unsigned int vao_;
  unsigned int vbo_;
  unsigned int ebo_;

  void SetupMesh();
};

#endif
