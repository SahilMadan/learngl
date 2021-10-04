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

// Time between current frame and last frame
float delta_time = 0.0f;
// The time of the last frame
float last_frame = 0.0f;

// Mouse position
float mouse_last_x = 400;
float mouse_last_y = 300;
bool first_mouse_position = true;

Camera camera(glm::vec3(0.0f, 0.0f, 55.0f));

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

  Shader shader("shaders/23_3_asteroids.vs", "shaders/15_1_depth_testing.fs");
  Shader instanced_shader("shaders/23_4_asteroids_instanced.vs",
                          "shaders/15_1_depth_testing.fs");

  Model planet("assets/models/planet/planet.obj");
  Model rock("assets/models/rock/rock.obj");

  unsigned int amount = 5000;
  glm::mat4* model_matrices;
  model_matrices = new glm::mat4[amount];
  srand(glfwGetTime());
  float radius = 50.0f;
  float offset = 2.5f;
  for (unsigned int i = 0; i < amount; i++) {
    glm::mat4 model = glm::mat4(1.0f);
    // Translation: displace along circle with 'radius' in range [-offset,
    // offset]
    float angle = (float)i / (float)amount * 360.0f;
    float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float x = sin(angle) * radius + displacement;
    displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    // Keep height of asteroid field smaller compared to width of x and z
    float y = displacement * 0.4f;
    displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float z = cos(angle) * radius + displacement;
    model = glm::translate(model, glm::vec3(x, y, z));

    // Scale: scale between 0.05 and 0.25
    float scale = (rand() % 20) / 100.0f + 0.05f;
    model = glm::scale(model, glm::vec3(scale));

    // Rotationn: Add random rotationn around a (semi) randomly picked rotation
    // axis vector
    float rotation_angle = (rand() % 360);
    model = glm::rotate(model, rotation_angle, glm::vec3(0.4f, 0.6f, 0.8f));

    // Now add to list of matrices
    model_matrices[i] = model;
  }

  // Vertex Buffer for instance matrices
  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &model_matrices[0],
               GL_STATIC_DRAW);

  for (unsigned int i = 0; i < rock.Meshes().size(); i++) {
    // We want to instance each MESH (since each mesh has its own VAO).
    unsigned int vao = rock.Meshes()[i].vao();
    glBindVertexArray(vao);
    // Vertex attributes
    // NOTE: The maximum amount allowed for a vertex attribute is a vec4.
    // Because mat4 are basically 4 vec4s, we have to reserve 4 vertex
    // attributes for this specific matrix - 3, 4, 5, and 6.
    std::size_t vec4_size = sizeof(glm::vec4);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size,
                          (void*)(1 * vec4_size));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size,
                          (void*)(2 * vec4_size));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size,
                          (void*)(3 * vec4_size));
    glEnableVertexAttribArray(6);

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
  }

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

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    // Draw planets
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
    shader.SetMat4("model", model);
    planet.Draw(shader);

    // Draw rocks
    instanced_shader.Use();
    instanced_shader.SetInt("texture1", 0);
    instanced_shader.SetMat4("view", view);
    instanced_shader.SetMat4("projection", projection);
    for (unsigned int i = 0; i < rock.Meshes().size(); i++) {
      glBindVertexArray(rock.Meshes()[i].vao());
      glDrawElementsInstanced(GL_TRIANGLES, rock.Meshes()[i].indices.size(),
                              GL_UNSIGNED_INT, 0, amount);
    }

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
