#include <glad/glad.h>
// Do not sort above glad
#include <GLFW/glfw3.h>

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
void DrawScene(Shader& shader, unsigned int cube_vao, unsigned int cube_texture,
               unsigned int plane_vao, unsigned int floor_texture);

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

  // Tell stb_image.h to flip loaded texture's on the y-axis (before loading
  // model).
  stbi_set_flip_vertically_on_load(true);

  Shader shader("shaders/15_1_depth_testing.vs",
                "shaders/15_1_depth_testing.fs");
  Shader screen_shader("shaders/19_1_framebuffers.vs",
                       "shaders/19_5_framebuffers_kernel_edge_detection.fs");

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

  // Vertex for quad that fills the entire screen in normalized device
  // coordinates. Position (x, y), Texture Coordinates (s, t).
  float quad_vertices[] = {
      -1.0f, 1.0f,  0.0f, 1.0f,  //
      -1.0f, -1.0f, 0.0f, 0.0f,  //
      1.0f,  -1.0f, 1.0f, 0.0f,  //

      -1.0f, 1.0f,  0.0f, 1.0f,  //
      1.0f,  -1.0f, 1.0f, 0.0f,  //
      1.0f,  1.0f,  1.0f, 1.0f   //
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

  // Quad VAO
  unsigned int quad_vao;
  glGenVertexArrays(1, &quad_vao);
  glBindVertexArray(quad_vao);

  unsigned int quad_vbo;
  glGenBuffers(1, &quad_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Load textures
  auto cube_texture = LoadTexture("assets/textures/container.jpg");
  auto floor_texture = LoadTexture("assets/textures/metal.png");

  // Create a framebuffer
  unsigned int fbo;
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  // To use the framebuffer we need to...
  // Attach at least one buffer (color, depth or stencil)
  // There should be at least one color attachment
  // All attachments should be complete as well (reserved memory)
  // Each buffer should have the same number of samples

  // Texture attachment
  unsigned int texture_color_buffer;
  glGenTextures(1, &texture_color_buffer);
  glBindTexture(GL_TEXTURE_2D, texture_color_buffer);
  // Set width/height to screen dimensions (though we don't have to), and pass
  // NULL as the texture's data parameter.
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texture_color_buffer, 0);

  // Renderbuffer attachment:
  // Renderbuffers are highly optimized for writing - that means they cannot be
  // read from directly. This gives them a performance edge over framebuffers
  // for off-screen rendering. You don't need to do any conversion to a
  // texture-specific format - their data format is native. That also means
  // swapping renderbuffer objects is fast. Note that glfwSwapBuffers is likely
  // implemented using renderbuffer objects.

  // Since we don't need to sample the depth/stencil buffers, we can use a rbo.
  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  // Finally, attach the renderbuffer object to the depth and stencil attachment
  // of the framebuffer.
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, rbo);

  // Check if we actually successfully completed the framebuffer
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "Unable to complete framebuffer\n";
    return 1;
  }

  // To render to the original framebuffer again
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

    // First Pass
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    DrawScene(shader, cube_vao, cube_texture, plane_vao, floor_texture);

    // Second Pass
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    screen_shader.Use();
    glBindVertexArray(quad_vao);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, texture_color_buffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Swap buffers and poll I/O events (keys pressed, mouse moved, etc.)
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Cleanup
  glDeleteFramebuffers(1, &fbo);

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

void DrawScene(Shader& shader, unsigned int cube_vao, unsigned int cube_texture,
               unsigned int plane_vao, unsigned int floor_texture) {
  shader.Use();
  shader.SetInt("texture1", 0);

  // Using lookAt...
  auto view = camera.GetViewMatrix();
  shader.SetMat4("view", view);

  // Use perspective projection
  glm::mat4 projection;
  projection = glm::perspective(
      glm::radians(camera.GetFieldOfView()),
      static_cast<float>(kScreenWidth) / static_cast<float>(kScreenHeight),
      0.1f, 100.0f);
  shader.SetMat4("projection", projection);

  auto model = glm::mat4(1.0f);

  // Cubes
  glBindVertexArray(cube_vao);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, cube_texture);
  model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
  shader.SetMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
  shader.SetMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // Floor
  glBindVertexArray(plane_vao);
  glBindTexture(GL_TEXTURE_2D, floor_texture);
  shader.SetMat4("model", glm::mat4(1.0f));
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}
