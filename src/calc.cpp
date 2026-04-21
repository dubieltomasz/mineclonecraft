#include "../include/calc.hpp"

namespace calc {
Vec2::Vec2() {
  this->x = 0;
  this->y = 0;
}

Vec2::Vec2(const float& x1, const float& y1, const float& x2, const float& y2) {
  this->x = x2 - x1;
  this->y = y2 - y1;
}

Vec2::Vec2(float theta) {
  theta *= 2.0 * M_PI;
  this->x = std::cos(theta);
  this->y = std::sin(theta);
}

Vec2::Vec2(float x, float y) {
  this->x = x;
  this->y = y;
}

float Vec2::operator*(const Vec2 &v) const {
  return this->x * v.x + this->y * v.y;
}

Vec4::Vec4(float x, float y, float z, float w) {
  this->x = x;
  this->y = y;
  this->z = z;
  this->w = w;
}

Vec4::Vec4() {
  this->x = 0.0f;
  this->y = 0.0f;
  this->z = 0.0f;
  this->w = 0.0f;
}

Vec4::Vec4(std::array<float, 4> values) {
  this->x = values[0];
  this->y = values[1];
  this->z = values[2];
  this->w = values[3];
}

Vec4::Vec4(float value) {
  this->x = value;
  this->y = value;
  this->z = value;
  this->w = value;
}

float Vec4::magnitude() const {
  return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z +
                    this->w * this->w);
}

Vec4 Vec4::operator+(const Vec4 &v) const {
  return {this->x + v.x, this->y + v.y, this->z + v.z, this->w + v.w};
}

Vec4 Vec4::operator-(const Vec4 &v) const {
  return {this->x - v.x, this->y - v.y, this->z - v.z, this->w - v.w};
}

template <typename T> Vec4 Vec4::operator*(const T &value) const {
  return {this->x * value, this->y * value, this->z * value, this->w * value};
}

template <typename T> Vec4 Vec4::operator/(const T &value) const {
  return {this->x / value, this->y / value, this->z / value, this->w / value};
}

float Vec4::dotProduct(const Vec4 &v1, const Vec4 &v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

Vec4 Vec4::normalize(const Vec4 &v) {
  float mag = std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);

  if(mag < 1e-6f) {
    return {0, 0, 0, 0};
  } else {
    return {v.x / mag, v.y / mag, v.z / mag, 0.0f};
  }
}

Vec4 Vec4::crossProduct(const Vec4 &v1, const Vec4 &v2) {
  return {
    v1.y * v2.z - v1.z * v2.y,
    v1.z * v2.x - v1.x * v2.z,
    v1.x * v2.y - v1.y * v2.x,
    0.0f
  };
}

bool Vec4::operator==(const Vec4 &v) const {
  return this->x == v.x && this->y == v.y && this->z == v.z && this->w == v.w;
}

float &Vec4::operator[](const int index) {
  switch (index) {
  case 0:
    return this->x;
    break;
  case 1:
    return this->y;
    break;
  case 2:
    return this->z;
    break;
  default:
    return this->w;
    break;
  }
}

float Vec4::operator[](const int index) const {
  switch (index) {
  case 0:
    return this->x;
    break;
  case 1:
    return this->y;
    break;
  case 2:
    return this->z;
    break;
  default:
    return this->w;
    break;
  }
}

Mat4::Mat4() {
  *this = MIdentity();
}

Mat4::Mat4(float value) {
  this->array.fill(0.0f);
  this->array[0] = value;
  this->array[5] = value;
  this->array[10] = value;
  this->array[15] = value;
}

Mat4::Mat4(const std::array<float, 16> &array) {
  std::copy(array.begin(), array.end(), this->array.data());
}

Mat4::Mat4(std::initializer_list<float> values) {
  if (values.size() != 16) {
    this->array.fill(0.0f);
  } else {
    std::copy(values.begin(), values.end(), this->array.begin());
  }
}

float &Mat4::operator[](int index) { return this->array[index]; }

const float &Mat4::operator[](int index) const { return this->array[index]; }

float Mat4::operator()(int row, int col) const {
  return this->array[row * 4 + col];
}

float &Mat4::operator()(int row, int col) { return this->array[row * 4 + col]; }

Mat4 Mat4::operator+(const Mat4 &matrix) const {
  Mat4 array(0.0f);

  for (int i = 0; i < 16; ++i) {
    array[i] = this->array[i] + matrix[i];
  }

  return array;
}

Mat4 Mat4::operator-(const Mat4 &matrix) const {
  Mat4 array(0.0f);

  for (int i = 0; i < 16; ++i) {
    array[i] = this->array[i] - matrix[i];
  }

  return array;
}

template <typename T> Mat4 Mat4::operator*(const T &value) const {
  Mat4 array(0.0f);

  for (int i = 0; i < 16; ++i) {
    array[i] = this->array[i] * value;
  }

  return array;
}

Mat4 Mat4::operator*(const Mat4 &matrix) const {
  Mat4 array(0.0f);

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      for (int k = 0; k < 4; ++k) {
        array[i * 4 + j] += this->array[i * 4 + k] * matrix[k * 4 + j];
      }
    }
  }

  return array;
}

Vec4 Mat4::operator*(const Vec4 &vector) const {
  Vec4 result(0.0f);

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      result[i] += this->array[i * 4 + j] * vector[j];
    }
  }

  return result;
}

Mat4 Mat4::transpose() const {
  Mat4 array(0.0f);

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      array[i * 4 + j] = this->array[j * 4 + i];
    }
  }

  return array;
}

Mat4 Mat4::MIdentity() { return Mat4(1.0f); }

Mat4 Mat4::MRotationX(float theta) {
  Mat4 matrix(1.0f);

  matrix[5] = std::cos(theta);
  matrix[6] = -std::sin(theta);
  matrix[9] = std::sin(theta);
  matrix[10] = std::cos(theta);

  return matrix;
}

Mat4 Mat4::MRotationY(float theta) {
  Mat4 matrix(1.0f);

  matrix[0] = std::cos(theta);
  matrix[2] = std::sin(theta);
  matrix[8] = -std::sin(theta);
  matrix[10] = std::cos(theta);

  return matrix;
}

Mat4 Mat4::MRotationZ(float theta) {
  Mat4 matrix(1.0f);

  matrix[0] = std::cos(theta);
  matrix[1] = -std::sin(theta);
  matrix[4] = std::sin(theta);
  matrix[5] = std::cos(theta);

  return matrix;
}

Mat4 Mat4::lookAt(const Vec4 &v1, const Vec4 &v2, const Vec4 &v3) {
  Vec4 f = Vec4::normalize({v2.x - v1.x, v2.y - v1.y, v2.z - v1.z, 0.0f});
  Vec4 up = Vec4::normalize(v3);
  Vec4 s = Vec4::normalize(Vec4::crossProduct(f, up));
  Vec4 u = Vec4::crossProduct(s, f);

  Mat4 result(1.0f);

  result(0,0) = s.x;
  result(1,0) = s.y;
  result(2,0) = s.z;

  result(0,1) = u.x;
  result(1,1) = u.y;
  result(2,1) = u.z;

  result(0,2) = -f.x;
  result(1,2) = -f.y;
  result(2,2) = -f.z;

  result(3,0) = -Vec4::dotProduct(s, v1);
  result(3,1) = -Vec4::dotProduct(u, v1);
  result(3,2) = Vec4::dotProduct(f, v1);

  result(3, 3) = 1.0f;

  return result;
}

Mat4 Mat4::perspective(float fov, float f1, float f2, float f3) {
  float tanHalfFov = std::tan(fov / 2.0f);

  Mat4 result(0.0f);

  result(0,0) = 1.0f / (f1 * tanHalfFov);
  result(1,1) = 1.0f / tanHalfFov;
  result(2,2) = f3 / (f2 - f3);
  result(2,3) = 1.0f;
  result(3,2) = -(f3 * f2) / (f3 - f2);

  return result;
}
} // namespace calc
