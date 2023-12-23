// Copyright (c) 2023.

//
// Created by pyxiion on 04.11.23.
//

#include "camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../engine.hpp"

namespace px {
  Camera::Camera()
    : m_renderer(getWorld().getEngine().getRenderer())
    , m_yaw(0), m_pitch(0) {

  }

  void Camera::perspective(float fovy, float aspect, float near, float far) {
    setProjection(glm::perspective(glm::radians(fovy), aspect, near, far));
  }

  void Camera::setProjection(Matrix4x4 projection) {
    m_proj = projection;
  }

  Matrix4x4 Camera::getProjection() const {
    return m_proj;
  }

  void Camera::lookAt(Vector3 pos) {
    m_direction = glm::normalize(pos - m_position);
    setRotation(m_direction);
  }

  void Camera::lookAt(float x, float y, float z) {
    lookAt({x, y, z});
  }

  void Camera::setPosition(Vector3 pos) {
    m_position = pos;
    m_changed = true;
  }

  void Camera::setPosition(float x, float y, float z) {
    setPosition({x, y, z});
  }

  Vector3 Camera::getPosition() const {
    return m_position;
  }

  void Camera::move(Vector3 offset) {
    m_position += offset;
    m_changed = true;
  }

  void Camera::move(float offsetX, float offsetY, float offsetZ) {
    move({offsetX, offsetY, offsetZ});
  }

  void Camera::setRotation(float yaw, float pitch) {
    m_yaw = yaw;
    m_pitch = pitch;

    m_changedAngles = true;
  }

  void Camera::setRotation(Vector3 direction) {
    m_direction = direction;
    setRotation(
        glm::degrees(glm::atan(m_direction.y, m_direction.x)), // yaw
        glm::degrees(-std::asin(m_direction.z))                // pitch
        );
  }

  void Camera::updateRotation(float deltaYaw, float deltaPitch) {
    m_yaw += deltaYaw;
    m_pitch += deltaPitch;

    m_changedAngles = true;
  }

  void Camera::apply() {
    if (m_changed)
      recalculate();

    bgfx::setViewTransform(m_renderer.getViewId(), glm::value_ptr(m_view), glm::value_ptr(m_proj));
  }

  void Camera::recalculate() {
    Vector3 up = Vector3Up;

    if (m_changedAngles) {
      float pitch = glm::radians(-m_pitch);
      float yaw = glm::radians(m_yaw);

      m_direction.x = glm::cos(pitch) * glm::cos(-yaw);
      m_direction.y = glm::sin(pitch);
      m_direction.z = glm::cos(pitch) * glm::sin(yaw);

      m_changedAngles = false;
    }

    m_right = glm::normalize(glm::cross(up, m_direction));
    m_up = glm::cross(m_direction, m_right);
    m_view = glm::lookAt(m_position, m_position + m_direction, m_up);

    m_changed = false;
  }

  Vector3 Camera::getUp() {
    if (m_changed)
      recalculate();
    return m_up;
  }

  Vector3 Camera::getRight() {
    if (m_changed)
      recalculate();
    return m_right;
  }

  Vector3 Camera::getForward() {
    if (m_changed)
      recalculate();
    return m_direction;
  }

  void Camera::guiEditor() {
    bool edited = false;

    edited |= ImGui::InputVector3("Position",  m_position,  "%.2f");
    edited |= ImGui::InputFloat2("Rotation",  &m_yaw,       "%.1f");

    // update direction
    if (ImGui::InputVector3("Direction", m_direction, "%.1f")) {
      setRotation(m_direction);
    }

    edited |= ImGui::InputVector3("Up",        m_up,        "%.1f", ImGuiInputTextFlags_ReadOnly);
    edited |= ImGui::InputVector3("Right",     m_right,     "%.1f", ImGuiInputTextFlags_ReadOnly);

    if (edited)
      recalculate();
  }
} // px