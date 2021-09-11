#include <glad/glad.h>
// Do not sort above glad
#include <GLFW/glfw3.h>

#include <glm/ext/matrix_transform.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "camera.hpp"
#include "model.hpp"
#include "shader_m.hpp"
#include "stb_include.hpp"

// Default settings
constexpr unsigned int kScreenWidth = 800;
constexpr unsigned int kScreenHeight = 600;

// Function declarations
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void MouseCursorCallback(GLFWwindow* window, double x_position,
                         double y_position);
void MouseScrollCallback(GLFWwindow* window, double x_offset, double y_offset);
unsigned int LoadTexture(char const* path);

// Time between current frame and last frame
float delta_time = 0.0f;
// The time of the last frame
float last_frame = 0.0f;

// Mouse position
float mouse_last_x = 400;
float mouse_last_y = 300;
bool first_mouse_position = true;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

int main() {
  // Initialize and configure GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a GLFW window
  GLFWwindow* window = glfwCreateWindow(kScreenWidth, kScreenHeight,
                                        "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
  glfwSetCursorPosCallback(window, MouseCursorCallback);
  glfwSetScrollCallback(window, MouseScrollCallback);

  // Disable the cursor and capture it
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Load all OpenGL function pointers
  // Note that this must be called after MakeContextCurrent
  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    std::cerr << "Failed to initialize GLAD\n";
    return -1;
  }

  // Configure global OpenGL state to include z-buffer depth testing
  glEnable(GL_DEPTH_TEST);

  // Configure the OpenGL state to include stencil testing. From this point on,
  // all rendering calls will influence the stencil buffer in one way or
  // another.
  glEnable(GL_STENCIL_TEST);

  glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

  // Specify how the values are written to the stencil buffer.
  // We want to KEEP the value if the stencil test fails
  // We want to KEEP the value if the stencil test passes but depth fails
  // We want to REPLACE the value if the stencil and depth test pass
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  // Tell stb_image.h to flip loaded texture's on the y-axis (before loading
  // model).
  stbi_set_flip_vertically_on_load(true);

  Shader shader("shaders/15_1_depth_testing.vs",
                "shaders/15_1_depth_testing.fs");

  Shader single_color_shader("shaders/15_1_depth_testing.vs",
                             "shaders/16_1_stencil_buffer.fs");

  // Set up vertex data and buffers and configure vertex attributes
  // Cube vertices: position (x, y, z), texture coordinates (s, t)
  float cube_vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  //
      0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,  //
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,  //
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  //

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  //
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  //
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  //
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f,  //
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  //
      -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,  //
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //
      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  //

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,  //
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,  //
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f,  //
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
      0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,  //
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  //
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  //
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,  //
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f,  //
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f   //
  };

  // Position (x, y, z), texture coordinates (s, t)
  // Note: Texture coordinates set higher than (together with GL_REPEAT) to
  // cause floor repeat.
  float plane_vertices[] = {
      5.0f,  -0.5f, 5.0f,  2.0f, 0.0f,  //
      -5.0f, -0.5f, 5.0f,  0.0f, 0.0f,  //
      -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,  //

      5.0f,  -0.5f, 5.0f,  2.0f, 0.0f,  //
      -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,  //
      5.0f,  -0.5f, -5.0f, 2.0f, 2.0f   //
  };

  // Cube VAO
  unsigned int cube_vao;
  glGenVertexArrays(1, &cube_vao);
  glBindVertexArray(cube_vao);

  unsigned int cube_vbo;
  glGenBuffers(1, &cube_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), &cube_vertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  // Plane VAO
  unsigned int plane_vao;
  glGenVertexArrays(1, &plane_vao);
  glBindVertexArray(plane_vao);

  unsigned int plane_vbo;
  glGenBuffers(1, &plane_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, plane_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), &plane_vertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  // Load textures
  auto cube_texture = LoadTexture("assets/textures/marble.jpg");
  auto floor_texture = LoadTexture("assets/textures/metal.png");

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    // Calculate delta time
    // People's machines have different processing powers and are able to render
    // much more frames. This results in some people moving really fast and
    // others really slow. To account for this, we should calculate
    // physics/movement based on the time difference between the two frames.
    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    // Input
    ProcessInput(window);

    // Render
    glEnable(GL_DEPTH_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Set uniforms

    // Using lookAt...
    auto view = camera.GetViewMatrix();

    // Use perspective projection
    glm::mat4 projection;
    projection = glm::perspective(
        glm::radians(camera.GetFieldOfView()),
        static_cast<float>(kScreenWidth) / static_cast<float>(kScreenHeight),
        0.1f, 100.0f);

    single_color_shader.Use();
    single_color_shader.SetMat4("view", view);
    single_color_shader.SetMat4("projection", projection);
    shader.Use();
    shader.SetInt("texture1", 0);
    shader.SetMat4("view", view);
    shader.SetMat4("projection", projection);

    auto model = glm::mat4(1.0f);

    // Draw the floor

    // Set the stencil mask as 0x00 so that it does not change the stencil
    // buffer
    glStencilMask(0x00);
    glBindVertexArray(plane_vao);
    glBindTexture(GL_TEXTURE_2D, floor_texture);
    shader.SetMat4("model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    // 1st render pass: Draw cubes as normal, writing to stencil buffer

    // Set the stencil test function that determines if pass or discard.
    // Always pass on first pass as we always want to draw the cubes.
    glStencilFunc(GL_ALWAYS, 1, 0xFF);

    // Set the stencil mask to write 0xFF to stencil buffer
    glStencilMask(0xFF);

    glBindVertexArray(cube_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cube_texture);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    shader.SetMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    shader.SetMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 2nd render pass: Now draw slightly scaled versions of cubes, this time
    // disabling stencil writing. Because the stencil buffer is now filled with
    // several 1s. THe parts of the buffer that are 1 are not drawn, thus only
    // drawing the cubes' size differences, making it look like borders.

    single_color_shader.Use();

    // Set the stencil mask to 0 (we don't want to write to buffer).
    glStencilMask(0x00);

    // Set the stencil test function to discard if equal to 0xFF.
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

    glDisable(GL_DEPTH_TEST);

    float scale = 1.1;
    glBindVertexArray(cube_vao);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    single_color_shader.SetMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    single_color_shader.SetMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);

    // Swap buffers and poll I/O events (keys pressed, mouse moved, etc.)
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Terminate, clearing all previously allocated GLFW resources
  glfwTerminate();
  return 0;
}

void ProcessInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
    return;
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.ProcessMovement(CameraMovement::FORWARD, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.ProcessMovement(CameraMovement::BACKWARD, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.ProcessMovement(CameraMovement::LEFT, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.ProcessMovement(CameraMovement::RIGHT, delta_time);
  }
}

void FramebufferSizeCallback(GLFWwindow*, int width, int height) {
  // Make sure the viewport matches the new window dimensions.
  glViewport(0, 0, width, height);
}

void MouseCursorCallback(GLFWwindow*, double x_position, double y_position) {
  if (first_mouse_position) {
    first_mouse_position = false;
    mouse_last_x = x_position;
    mouse_last_y = y_position;
    return;
  }

  float x_offset = x_position - mouse_last_x;
  float y_offset = (y_position - mouse_last_y) * -1;
  mouse_last_x = x_position;
  mouse_last_y = y_position;

  camera.ProcessLook(x_offset, y_offset);
}

void MouseScrollCallback(GLFWwindow*, double /*x_offset*/, double y_offset) {
  camera.ProcessFieldOfView(y_offset);
}

unsigned int LoadTexture(char const* path) {
  unsigned int texture_id;
  glGenTextures(1, &texture_id);

  int width;
  int height;
  int component_count;
  auto* data = stbi_load(path, &width, &height, &component_count, 0);
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
    };

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  } else {
    std::cerr << "Texture failed to load at path: " << path << "\n";
    stbi_image_free(data);
  }

  return texture_id;
}
