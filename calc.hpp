#include <array>
#include <cmath>
#include <initializer_list>
#include <math.h>

namespace calc {
class Vec4 {
public:
  float x, y, z, w;

  Vec4(float x, float y, float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  }

  Vec4(std::array<float, 4> values) {
    this->x = values[0];
    this->y = values[1];
    this->z = values[2];
    this->w = values[3];
  }

  Vec4(float value) {
    this->x = value;
    this->y = value;
    this->z = value;
    this->w = value;
  }

  float magnitude() const {
    return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z +
                     this->w * this->w);
  }

  Vec4 operator+(const Vec4 &v) const {
    return {this->x + v.x, this->y + v.y, this->z + v.z, this->w + v.w};
  }

  Vec4 operator-(const Vec4 &v) const {
    return {this->x - v.x, this->y - v.y, this->z - v.z, this->w - v.w};
  }

  template <typename T> Vec4 operator*(const T &value) const {
    return {this->x * value, this->y * value, this->z * value, this->w * value};
  }

  template <typename T> Vec4 operator/(const T &value) const {
    return {this->x / value, this->y / value, this->z / value, this->w / value};
  }

  static float dotProduct(const Vec4 &v1, const Vec4 &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
  }

  bool operator==(const Vec4 &v) const {
    return this->x == v.x && this->y == v.y && this->z == v.z && this->w == v.w;
  }

  float &operator[](const int index) {
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

  float operator[](const int index) const {
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
};

class Mat4 {
public:
  std::array<float, 16> array;

  Mat4(float value) {
    this->array.fill(0.0f);
    this->array[0] = value;
    this->array[5] = value;
    this->array[10] = value;
    this->array[15] = value;
  }

  Mat4(const std::array<float, 16> &array) {
    std::copy(array.begin(), array.end(), this->array.data());
  }

  Mat4(std::initializer_list<float> values) {
    if (values.size() != 16) {
      this->array.fill(0.0f);
    } else {
      std::copy(values.begin(), values.end(), this->array.begin());
    }
  }

  Vec4 a() {
    return {this->array[0], this->array[1], this->array[2], this->array[3]};
  }

  Vec4 b() {
    return {this->array[4], this->array[5], this->array[6], this->array[7]};
  }

  Vec4 c() {
    return {this->array[8], this->array[9], this->array[10], this->array[11]};
  }

  Vec4 d() {
    return {this->array[12], this->array[13], this->array[14], this->array[15]};
  }

  Vec4 x() {
    return {this->array[0], this->array[4], this->array[8], this->array[12]};
  }

  Vec4 y() {
    return {this->array[1], this->array[5], this->array[9], this->array[13]};
  }

  Vec4 z() {
    return {this->array[2], this->array[6], this->array[10], this->array[14]};
  }

  Vec4 w() {
    return {this->array[3], this->array[7], this->array[11], this->array[15]};
  }

  float &operator[](int index) { return this->array[index]; }

  const float &operator[](int index) const { return this->array[index]; }

  float operator()(int row, int col) const {
    return this->array[row * 4 + col];
  }

  float &operator()(int row, int col) { return this->array[row * 4 + col]; }

  Mat4 operator+(const Mat4 &matrix) const {
    Mat4 array(0.0f);

    for (int i = 0; i < 16; ++i) {
      array[i] = this->array[i] + matrix[i];
    }

    return array;
  }

  Mat4 operator-(const Mat4 &matrix) const {
    Mat4 array(0.0f);

    for (int i = 0; i < 16; ++i) {
      array[i] = this->array[i] - matrix[i];
    }

    return array;
  }

  template <typename T> Mat4 operator*(const T &value) const {
    Mat4 array(0.0f);

    for (int i = 0; i < 16; ++i) {
      array[i] = this->array[i] * value;
    }

    return array;
  }

  Mat4 operator*(const Mat4 &matrix) const {
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

  Vec4 operator*(const Vec4 &vector) const {
    Vec4 result(0.0f);

    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        result[i] += this->array[i * 4 + j] * vector[j];
      }
    }

    return result;
  }

  Mat4 transpose() const {
    Mat4 array(0.0f);

    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        array[i * 4 + j] = this->array[j * 4 + i];
      }
    }

    return array;
  }

  static Mat4 MIdentity() { return Mat4(1.0f); }

  static Mat4 MRotationX(float theta) {
    Mat4 matrix(1.0f);

    matrix[5] = std::cos(theta);
    matrix[6] = -std::sin(theta);
    matrix[9] = std::sin(theta);
    matrix[10] = std::cos(theta);

    return matrix;
  }

  static Mat4 MRotationY(float theta) {
    Mat4 matrix(1.0f);

    matrix[0] = std::cos(theta);
    matrix[3] = std::sin(theta);
    matrix[8] = -std::sin(theta);
    matrix[10] = std::cos(theta);

    return matrix;
  }

  static Mat4 MRotationZ(float theta) {
    Mat4 matrix(1.0f);

    matrix[0] = std::cos(theta);
    matrix[1] = -std::sin(theta);
    matrix[4] = std::sin(theta);
    matrix[5] = std::cos(theta);

    return matrix;
  }
};
} // namespace calc
