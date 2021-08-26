#ifndef LEARNGL_SHADER_M_H_
#define LEARNGL_SHADER_M_H_

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <iostream>

class Shader {
 public:
  Shader(const char* vertex_path, const char* fragment_path);
  unsigned int id() { return id_; }
  void Use() const;

  void SetBool(const std::string& name, bool value) const;
  void SetInt(const std::string& name, int value) const;
  void SetFloat(const std::string& name, float value) const;

  void SetVec2(const std::string& name, const glm::vec2& value) const;
  void SetVec3(const std::string& name, const glm::vec3& value) const;
  void SetVec3(const std::string& name, float x, float y, float z) const;
  void SetVec4(const std::string& name, const glm::vec4& value) const;

  void SetMat2(const std::string& name, const glm::mat2& value) const;
  void SetMat3(const std::string& name, const glm::mat3& value) const;
  void SetMat4(const std::string& name, const glm::mat4& value) const;

 private:
  unsigned int id_;
  unsigned int CompileShader(GLenum shader_type, const char* path);
};

#endif
