#include <glad/glad.h>
// Do not sort above glad
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "camera.hpp"
#include "shader_m.hpp"

// Default settings
constexpr unsigned int kScreenWidth = 800;
constexpr unsigned int kScreenHeight = 600;

// Function declarations
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void MouseCursorCallback(GLFWwindow* window, double x_position,
                         double y_position);
void MouseScrollCallback(GLFWwindow* window, double x_offset, double y_offset);

// Time between current frame and last frame
float delta_time = 0.0f;
// The time of the last frame
float last_frame = 0.0f;

// Mouse position
float mouse_last_x = 400;
float mouse_last_y = 300;
bool first_mouse_position = true;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

const glm::vec3 kLightPos(1.2f, 1.0f, 2.0f);

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

  // Set up vertex data (and buffer(s)) and configure vertex attributes
  // Bind the vertex array object first, then bind and set the vertex buffer(s),
  // then configure the vertex attribute(s)

  // Position (x, y, z)
  float vertices[] = {
      -0.5f, -0.5f, -0.5f,  //
      0.5f,  -0.5f, -0.5f,  //
      0.5f,  0.5f,  -0.5f,  //
      0.5f,  0.5f,  -0.5f,  //
      -0.5f, 0.5f,  -0.5f,  //
      -0.5f, -0.5f, -0.5f,  //

      -0.5f, -0.5f, 0.5f,  //
      0.5f,  -0.5f, 0.5f,  //
      0.5f,  0.5f,  0.5f,  //
      0.5f,  0.5f,  0.5f,  //
      -0.5f, 0.5f,  0.5f,  //
      -0.5f, -0.5f, 0.5f,  //

      -0.5f, 0.5f,  0.5f,   //
      -0.5f, 0.5f,  -0.5f,  //
      -0.5f, -0.5f, -0.5f,  //
      -0.5f, -0.5f, -0.5f,  //
      -0.5f, -0.5f, 0.5f,   //
      -0.5f, 0.5f,  0.5f,   //

      0.5f,  0.5f,  0.5f,   //
      0.5f,  0.5f,  -0.5f,  //
      0.5f,  -0.5f, -0.5f,  //
      0.5f,  -0.5f, -0.5f,  //
      0.5f,  -0.5f, 0.5f,   //
      0.5f,  0.5f,  0.5f,   //

      -0.5f, -0.5f, -0.5f,  //
      0.5f,  -0.5f, -0.5f,  //
      0.5f,  -0.5f, 0.5f,   //
      0.5f,  -0.5f, 0.5f,   //
      -0.5f, -0.5f, 0.5f,   //
      -0.5f, -0.5f, -0.5f,  //

      -0.5f, 0.5f,  -0.5f,  //
      0.5f,  0.5f,  -0.5f,  //
      0.5f,  0.5f,  0.5f,   //
      0.5f,  0.5f,  0.5f,   //
      -0.5f, 0.5f,  0.5f,   //
      -0.5f, 0.5f,  -0.5f   //
  };

  Shader shader("shaders/8_1_colors.vs", "shaders/8_1_colors.fs");

  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  unsigned int vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // This is allowed. The call to glVertexAttribPointer registered the VBO as
  // the vertex attribute's bound vbo so that afterwards we can safely unbind.
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // You can unbind the vao afterwards so that other vao calls won't
  // accidentally modify this vao, but this rarely happens.
  glBindVertexArray(0);

  // Light

  Shader light_cube_shader("shaders/8_1_colors.vs", "shaders/8_1_light.fs");

  unsigned int light_vao;
  glGenVertexArrays(1, &light_vao);
  glBindVertexArray(light_vao);

  // We only need to bind the VBO, the container's VBO's data already contains
  // the data
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

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
    shader.SetVec3("objectColor", 1.0f, 0.5f, 0.31f);
    shader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);

    // Camera

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

    glBindVertexArray(vao);

    glm::mat4 model = glm::mat4(1.0f);
    shader.SetMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Light
    light_cube_shader.Use();
    light_cube_shader.SetMat4("view", view);
    light_cube_shader.SetMat4("projection", projection);
    model = glm::mat4(1.0f);
    model = glm::translate(model, kLightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    light_cube_shader.SetMat4("model", model);
    glBindVertexArray(light_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Swap buffers and poll I/O events (keys pressed, mouse moved, etc.)
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Optional: De-allocate all resources once they've outlived their purpose
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);

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
