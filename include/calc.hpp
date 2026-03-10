#pragma once

#include <array>
#include <cmath>
#include <initializer_list>
#include <math.h>

namespace calc {
class Vec4 {
public:
  float x, y, z, w;

  Vec4(float x, float y, float z, float w);

  Vec4(std::array<float, 4> values);

  Vec4(float value);

  float magnitude() const;

  Vec4 operator+(const Vec4 &v) const;

  Vec4 operator-(const Vec4 &v) const;

  template <typename T> Vec4 operator*(const T &value) const;
  template <typename T> Vec4 operator/(const T &value) const;

  static float dotProduct(const Vec4 &v1, const Vec4 &v2);

  bool operator==(const Vec4 &v) const;

  float &operator[](const int index);

  float operator[](const int index) const;
};

class Mat4 {
public:
  std::array<float, 16> array;

  Mat4();

  Mat4(float value);

  Mat4(const std::array<float, 16> &array);

  Mat4(std::initializer_list<float> values);

  float &operator[](int index);

  const float &operator[](int index) const;

  float operator()(int row, int col) const;

  float &operator()(int row, int col);

  Mat4 operator+(const Mat4 &matrix) const;

  Mat4 operator-(const Mat4 &matrix) const;

  template <typename T> Mat4 operator*(const T &value) const;

  Mat4 operator*(const Mat4 &matrix) const;

  Vec4 operator*(const Vec4 &vector) const;

  Mat4 transpose() const;

  static Mat4 MIdentity();

  static Mat4 MRotationX(float theta);

  static Mat4 MRotationY(float theta);

  static Mat4 MRotationZ(float theta);
};
} // namespace calc
