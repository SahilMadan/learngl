#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() : Camera(glm::vec3(0.0f, 0.0f, 0.0f)) {}

Camera::Camera(glm::vec3 position)
    : Camera(position, glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f) {}

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : position_(position),
      front_(glm::vec3(0.0f, 0.0f, -1.0f)),
      world_up_(up),
      yaw_(yaw),
      pitch_(pitch),
      movement_speed_(2.5f),
      look_sensitivity_(0.1f),
      field_of_view_(45.0f) {
  UpdateCameraVectors();
}

float Camera::GetFieldOfView() const { return field_of_view_; }

glm::mat4 Camera::GetViewMatrix() const {
  return glm::lookAt(position_, position_ + front_, up_);
}

glm::vec3 Camera::Position() const {
  return position_;
}

void Camera::UpdateCameraVectors() {
  // Imagine the yaw as an angle offset from the x-axis when looking at a
  // top-down view (looking at x/z plane). x = cos(yaw) and z = sin(yaw).
  // Similarly, looking at pitch from a side view (looking at y/z plane). z =
  // cos(yaw) and y = sin(yaw).
  // These can be used to calculate our new direction vector (aka front_).
  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  direction.y = sin(glm::radians(pitch_));
  direction.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  front_ = glm::normalize(direction);

  // Recalculate the right vector using cross product of world up and front.
  right_ = glm::normalize(glm::cross(front_, world_up_));
  // Using new right vector and front, we can recalculate the up vector.
  up_ = glm::normalize(glm::cross(right_, front_));
}

void Camera::ProcessMovement(CameraMovement movement, float delta_time) {
  const float velocity = movement_speed_ * delta_time;
  switch (movement) {
    case CameraMovement::FORWARD:
      position_ += front_ * velocity;
      break;
    case CameraMovement::BACKWARD:
      position_ -= front_ * velocity;
      break;
    case CameraMovement::LEFT:
      position_ -= right_ * velocity;
      break;
    case CameraMovement::RIGHT:
      position_ += right_ * velocity;
  }
}

void Camera::ProcessLook(float x_offset, float y_offset) {
  x_offset *= look_sensitivity_;
  y_offset *= look_sensitivity_;

  yaw_ += x_offset;
  pitch_ += y_offset;

  if (pitch_ > 89.0f) {
    pitch_ = 89.0f;
  } else if (pitch_ < -89.0f) {
    pitch_ = -89.0f;
  }

  UpdateCameraVectors();
}

void Camera::ProcessFieldOfView(float field_of_view_offset) {
  field_of_view_ -= field_of_view_offset;
  if (field_of_view_ < 1.0f) {
    field_of_view_ = 1.0f;
  } else if (field_of_view_ > 45.0f) {
    field_of_view_ = 45.0f;
  }
}
