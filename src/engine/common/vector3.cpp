#include "vector3.hpp"

px::Vector3::Vector3()
  : x(0.0f)
  , y(0.0f)
  , z(0.0f)
{}

px::Vector3::Vector3(float x, float y, float z)
  : x(x)
  , y(y)
  , z(z)
{}

float px::Vector3::dot(const px::Vector3 &other) const
{
  return x * other.x + y * other.y + z * other.z;
}

float px::Vector3::length() const
{
  return std::sqrt(x * x + y * y + z * z);
}

px::Vector3 px::Vector3::cross(const px::Vector3 &other) const
{
  return Vector3(
    y * other.z - z * other.y,
    z * other.x - x * other.z,
    x * other.y - y * other.x
  );
}

px::Vector3 px::Vector3::getRight(const px::Vector3 &up) const
{
  return cross(up);
}

px::Vector3 px::Vector3::getLeft(const px::Vector3 &up) const
{
  return -cross(up);
}

px::Vector3 px::Vector3::normalized() const
{
  return *this / length();
}

px::Vector3 px::Vector3::operator-() const
{
  return Vector3(-x, -y, -z);
}

px::Vector3 px::Vector3::operator+(const px::Vector3 &right) const
{
  return Vector3(x + right.x, y + right.y, z + right.z);
}

px::Vector3 px::Vector3::operator-(const px::Vector3 &right) const
{
  return Vector3(x - right.x, y - right.y, z - right.z);
}

px::Vector3 px::Vector3::operator*(float right) const
{
  return Vector3(x * right, y * right, z * right);
}

px::Vector3 px::Vector3::operator/(float right) const
{
  return Vector3(x / right, y / right, z / right);
}

px::Vector3 &px::Vector3::operator+=(const px::Vector3 &right)
{
  x += right.x;
  y += right.y;
  z += right.z;
  return *this;
}

px::Vector3 &px::Vector3::operator-=(const px::Vector3 &right)
{
  x -= right.x;
  y -= right.y;
  z -= right.z;
  return *this;
}

px::Vector3 &px::Vector3::operator*=(float right)
{
  x *= right;
  y *= right;
  z *= right;
  return *this;
}

px::Vector3 &px::Vector3::operator/=(float right)
{
  x /= right;
  y /= right;
  z /= right;
  return *this;
}

bool px::Vector3::operator==(const px::Vector3 &right) const
{
  return x == right.x && y == right.y && z == right.z;
}
