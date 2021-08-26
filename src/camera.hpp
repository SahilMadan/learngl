#ifndef LEARNGL_CAMERA_H_
#define LEARNGL_CAMERA_H_

#include <glm/glm.hpp>

enum class CameraMovement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
};

class Camera {
  public:
    Camera(); 
    Camera(glm::vec3 position);
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

    float GetFieldOfView() const;
    glm::mat4 GetViewMatrix() const;
    glm::vec3 Position() const;

    void ProcessMovement(CameraMovement movement, float delta_time);
    void ProcessLook(float x_offset, float y_offset);
    void ProcessFieldOfView(float field_of_view_offset);
    
  private:
    // Camera attributes
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_; 
    glm::vec3 right_;
    glm::vec3 world_up_;

    // Euler properties
    float yaw_;
    float pitch_;

    // Camera options
    float movement_speed_;
    float look_sensitivity_;
    float field_of_view_;

    void UpdateCameraVectors();
};

#endif
