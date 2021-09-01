#include "mesh.hpp"

#include <cstddef>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
           std::vector<Texture> textures) {
  this->vertices = vertices;
  this->indices = indices;
  this->textures = textures;

  SetupMesh();
}

void Mesh::SetupMesh() {
  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);
  glGenBuffers(1, &ebo_);

  glBindVertexArray(vao_);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0],
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               &indices[0], GL_STATIC_DRAW);

  // Vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  // Vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, normal));
  // Vertex texture coordinates
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, tex_coords));

  glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader) {
  unsigned int diffuse_count = 1;
  unsigned int specular_count = 1;

  for (unsigned int i = 0; i < textures.size(); i++) {
    // Activate proper texture unit before binding
    glActiveTexture(GL_TEXTURE0 + i);

    // Retrieve texture number (the N in diffuse_textureN);
    std::string number;
    std::string name = textures[i].type;
    if (name == "texture_diffuse") {
      number = std::to_string(diffuse_count++);
    } else if (name == "texture_specular") {
      number = std::to_string(specular_count++);
    }
    std::string fname = name + number;
    shader.SetFloat(fname, i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }

  // Draw mesh
  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

  // Always good practice to set everything back to defaults once configured
  glBindVertexArray(0);
  glActiveTexture(GL_TEXTURE0);
}
