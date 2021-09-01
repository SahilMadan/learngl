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
#include "model.hpp"
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

  Shader shader("shaders/14_1_model_loading.vs",
                "shaders/14_1_model_loading.fs");

  // Load models
  // Note: This model works in the current example if you download directly from
  // learnopengl.com (see Model Loading > Model). This is because, with .obj
  // files, we require both the obj and the mtl file (which specifies how to map
  // the textures to the object).
  Model backpack_model("assets/models/backpack/backpack.obj");
  std::cout << "Model loaded" << std::endl;

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
    shader.SetVec3("viewPosition", camera.Position());

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

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    shader.SetMat4("model", model);
    backpack_model.Draw(shader);

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
