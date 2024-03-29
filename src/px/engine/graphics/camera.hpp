// Copyright (c) 2023.

//
// Created by pyxiion on 04.11.23.
//

#ifndef ENGINE_CAMERA_HPP
#define ENGINE_CAMERA_HPP
#include "renderer.hpp"
#include "../math/matrix.hpp"
#include "../math/vector.hpp"
#include "../world/game_object.hpp"

namespace px {
  class Camera : public GameObject {
  public:
    PX_OBJECT_NAME("Camera #{}")

    explicit Camera();

    void perspective(float fovy, float aspect, float near = 0.1f, float far = 100.0f);
    void setProjection(Matrix4x4 projection);
    [[nodiscard]] Matrix4x4 getProjection() const;

    void setPosition(Vector3 pos);
    void setPosition(float x, float y, float z);
    [[nodiscard]] Vector3 getPosition() const;

    void lookAt(Vector3 pos);
    void lookAt(float x, float y, float z);

    void setRotation(Vector3 direction);
    void setRotation(float yaw, float pitch);
    void updateRotation(float deltaYaw, float deltaPitch);

    void move(Vector3 offset);
    void move(float offsetX, float offsetY, float offsetZ);

    Vector3 getUp();
    Vector3 getRight();
    Vector3 getForward();

    void apply();

    void guiEditor() override;

  private:
    Renderer &m_renderer;

    px::Vector3 m_position{};
    px::Vector3 m_direction{};

    float m_pitch, m_yaw;

    px::Vector3 m_up{};
    px::Vector3 m_right{};

    px::Matrix4x4 m_view{};
    px::Matrix4x4 m_proj{};

    bool m_changed = true;
    bool m_changedAngles = true;

    void recalculate();
  };

  typedef Ref<Camera> CameraPtr;

} // px

#endif //ENGINE_CAMERA_HPP
