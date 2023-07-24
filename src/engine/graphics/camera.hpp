#pragma once
#include <glm/glm.hpp>
#include "../math/vector.hpp"
#include "../math/matrix.hpp"

namespace px
{
  class Camera
  {
  public:
    Camera();

    void createPerspectiveProjectionMatrix(float FoV, float aspect, float near = 0.1f, float far = 100.0f);

    void rotateAt(Vector3 target, Vector3 up = Vector3Up);

    const Matrix4x4 &getProjectionMatrix();
    const Matrix4x4 &getViewMatrix();

  private:
    float m_pitch;
    float m_yaw;

    Matrix4x4 m_projection;
    Matrix4x4 m_view;
  };
}