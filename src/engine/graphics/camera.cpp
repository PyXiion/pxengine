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
    : m_renderer(getWorld().getEngine().getRenderer()) {

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
    m_direction = glm::normalize(m_position - pos);
    m_changed = true;
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

  void Camera::apply() {
    if (m_changed)
      recalculate();

    bgfx::setViewTransform(m_renderer.getViewId(), glm::value_ptr(m_view), glm::value_ptr(m_proj));

    auto size = m_renderer.getFramebufferSize();
    bgfx::setViewRect(m_renderer.getViewId(), 0, 0, uint16_t(size.x), uint16_t(size.y) );
  }

  void Camera::recalculate() {
    Vector3 up = Vector3Up;
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
} // px