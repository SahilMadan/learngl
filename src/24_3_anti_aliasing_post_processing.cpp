#include <glad/glad.h>
// Do not sort above glad
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "camera.hpp"
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

// MSAA: Toggle multisample
bool multisample_enabled = true;
bool multisample_key_trap = false;

int main() {
  // Initialize and configure GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

  // MSAA: We need to enable multisampling. NOTE: On most GL drivers, it is
  // enabled by default so this is a bit redundant.
  glEnable(GL_MULTISAMPLE);

  // Tell stb_image.h to flip loaded texture's on the y-axis (before loading
  // model).
  stbi_set_flip_vertically_on_load(true);

  Shader shader("shaders/24_1_anti_aliasing_msaa.vs",
                "shaders/24_1_anti_aliasing_msaa.fs");
  Shader screen_shader("shaders/19_1_framebuffers.vs",
                       "shaders/19_2_framebuffers_grayscale.fs");

  // Set up vertex data and buffers and configure vertex attributes
  // Cube vertices: position (x, y, z),
  float cube_vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f,
      0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,

      -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,

      -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,

      0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f,
      0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,

      -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,
      0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f,

      -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5};

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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
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

  // MSAA: Configure MSAA framebuffer
  // GLSL takes care of creating multisampled buffers, which makes MSAA easy.
  // But, if we want to use our own framebuffers, we have to generate
  // multisampled buffers ourselves.
  // See 19_* for more on framebuffers.
  unsigned int framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  // MSAA: Create a multisampled color attachment texture
  unsigned int texture_color_buffer_multisampled;
  glGenTextures(1, &texture_color_buffer_multisampled);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture_color_buffer_multisampled);
  constexpr int sample_count = 4;
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, sample_count, GL_RGB,
                          kScreenWidth, kScreenHeight, GL_TRUE);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

  // MSAA: Attach the multisampled texture to the framebuffer
  // The currently bound framebuffer now has a multisampled color buffer in the
  // form of a texture image.
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                         GL_TEXTURE_2D_MULTISAMPLE,
                         texture_color_buffer_multisampled, 0);

  // MSAA: Create a (also multisampled) renderbuffer object for depth and
  // stencil attachments
  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorageMultisample(GL_RENDERBUFFER, sample_count,
                                   GL_DEPTH24_STENCIL8, kScreenWidth,
                                   kScreenHeight);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, rbo);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "Error: Framebuffer is not complete!\n";
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // If we want to apply post-processing to the multisampled buffer, we can't
  // use the multisampled texture directly in the fragment shader. BUT, we can
  // blit to a different fbo.
  unsigned int intermediate_fbo;
  glGenFramebuffers(1, &intermediate_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, intermediate_fbo);

  // Create a color attachment texture which will be rendered to quad
  unsigned int screen_texture;
  glGenTextures(1, &screen_texture);
  glBindTexture(GL_TEXTURE_2D, screen_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, kScreenWidth, kScreenHeight, 0, GL_RGB,
               GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // Note: We only need a color buffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         screen_texture, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "Error: Intermediate framebuffer is not complete\n";
  }
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

    // Render: Draw as normal in multisampled buffers
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    shader.Use();

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

    glBindVertexArray(cube_vao);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -3.0f));
    shader.SetMat4("model", model);
    glBindVertexArray(cube_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Render: Blit the multisampled buffer to normal colorbuffer of
    // intermediate FBO. Image is stored in screen_texture.
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediate_fbo);
    // Blit transfers a given source region (defined by 4 screen coordinates) to
    // a target regionn (also defined by 4 screen coordinates).
    glBlitFramebuffer(0, 0, kScreenWidth, kScreenHeight, 0, 0, kScreenWidth,
                      kScreenHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    // Render: Quad with scene's visuals as its texture image
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    // Draw quad
    screen_shader.Use();
    screen_shader.SetInt("screenTexture", 0);
    glBindVertexArray(quad_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screen_texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

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

  // MSAA: Toggle multisampling on/off on x key press
  const auto x_key_state = glfwGetKey(window, GLFW_KEY_X);
  if (x_key_state == GLFW_PRESS && !multisample_key_trap) {
    multisample_key_trap = true;
    if (multisample_enabled) {
      glDisable(GL_MULTISAMPLE);
    } else {
      glEnable(GL_MULTISAMPLE);
    }
    multisample_enabled = !multisample_enabled;
  }
  if (multisample_key_trap && x_key_state == GLFW_RELEASE) {
    multisample_key_trap = false;
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
