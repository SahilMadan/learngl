#ifndef LEARNGL_SHADER_SIMPLE_H_
#define LEARNGL_SHADER_SIMPLE_H_

#include <glad/glad.h>

#include <iostream>

class Shader {
 public:
   Shader(const char* vertex_path, const char* fragment_path);
 unsigned int id() { return id_; }
 void Use() const;

 void SetBool(const std::string& name, bool value) const;
 void SetInt(const std::string& name, int value) const;
 void SetFloat(const std::string& name, float value) const;

 private:
  unsigned int id_;
  unsigned int CompileShader(GLenum shader_type, const char* path);
}; 

#endif
