#pragma once
#include <cmath>

namespace px
{

  /**
   * @brief Generic 3D vector class
   */
  struct Vector3 {
    Vector3();

    Vector3(float x, float y, float z);

    float dot(const Vector3 &other) const;

    float length() const;

    Vector3 cross(const Vector3 &other) const;

    Vector3 getRight(const Vector3 &up) const;

    Vector3 getLeft(const Vector3 &up) const;

    Vector3 normalized() const;

    Vector3 operator-() const;

    Vector3 operator+(const Vector3 &right) const;

    Vector3 operator-(const Vector3 &right) const;

    Vector3 operator*(float right) const;

    Vector3 operator/(float right) const;

    Vector3 &operator+=(const Vector3 &right);

    Vector3 &operator-=(const Vector3 &right);

    Vector3 &operator*=(float right);

    Vector3 &operator/=(float right);

    bool operator==(const Vector3 &right) const;

    float x;
    float y;
    float z;
  };

}

