#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace fileHandler {
constexpr uint32_t glTFmagicValue = 0x46546C67;
enum chunkType : uint32_t {
  JSON = 0x4E4F534A,
  BIN = 0x004E4942,
};

class ParseJSON {
public:
  std::unordered_map<std::string, std::string> um;

  int trimKey(const std::string &s);

  ParseJSON(const std::vector<unsigned char> &v);

  std::string get(const std::string &s);

  static std::string formatJSON(const std::vector<unsigned char> &v);
};

class glTFchunk {
public:
  uint32_t type;
  std::vector<unsigned char> data;

  glTFchunk(uint32_t type, std::vector<unsigned char> &&chunkData);

  static inline std::string typeToString(const uint32_t &type) {
    switch (type) {
    case chunkType::JSON:
      return "JSON";
    case chunkType::BIN:
      return "BIN";
    default:
      return "UNKNOWN";
    }
  }
};

class glTFmodel {
public:
  uint32_t magic;
  uint32_t version;
  uint32_t length;
  std::vector<glTFchunk> chunks;

  glTFmodel(const std::string &fileName);

  void info();

  std::vector<std::tuple<float, float, float>> getVertices();
};
} // namespace fileHandler
