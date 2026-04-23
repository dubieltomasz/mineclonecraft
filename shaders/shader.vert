#version 450

layout(binding = 0) uniform UniformBufferObject {
  mat4 model;
  mat4 trans;
  mat4 rot;
  mat4 proj;
} ubo;

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 fragColor;

void main() {
  gl_Position = ubo.proj * ubo.rot * ubo.trans * ubo.model * inPosition;
  fragColor = inColor;
}
