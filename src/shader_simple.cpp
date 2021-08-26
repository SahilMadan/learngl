#include "shader_simple.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Shader::Shader(const char* vertex_path, const char* fragment_path) {
  const auto vertex = CompileShader(GL_VERTEX_SHADER, vertex_path);
  const auto fragment = CompileShader(GL_FRAGMENT_SHADER, fragment_path);

  id_ = glCreateProgram();
  glAttachShader(id_, vertex);
  glAttachShader(id_, fragment);
  glLinkProgram(id_);

  int success;
  glGetProgramiv(id_, GL_LINK_STATUS, &success);
  if (!success) {
    char info_log[512];
    glGetProgramInfoLog(id_, 512, nullptr, info_log);
    std::cerr << "Program compilation failed:\n" << info_log << "\n";
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

unsigned int Shader::CompileShader(GLenum shader_type, const char* path) {
  std::ifstream shader_file;
  shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  std::string shader_content;
  try {
    shader_file.open(path);

    std::stringstream shader_stream;
    shader_stream << shader_file.rdbuf();

    shader_file.close();

    shader_content = shader_stream.str();
  } catch (std::ifstream::failure&) {
    std::cerr << "Shader file not read successfully\n";
    return 0;
  }

  const char* shader_content_c = shader_content.c_str();

  unsigned int shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, &shader_content_c, nullptr);
  glCompileShader(shader);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char info_log[512];
    glGetShaderInfoLog(shader, 512, nullptr, info_log);
    std::cerr << "Shader compilation failed (" << shader_type << "):\n"
              << info_log << "\n";
    return 0;
  }

  return shader;
}

void Shader::Use() const {
  glUseProgram(id_);
}

void Shader::SetBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(id_, name.c_str()), static_cast<int>(value)); 
}


void Shader::SetInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(id_, name.c_str()), value); 
}

void Shader::SetFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}
