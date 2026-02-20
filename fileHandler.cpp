#include <cstdint>
#include <fstream>
#include <print>
#include <vector>

namespace fileHandler {
constexpr uint32_t glTFmagicValue = 0x46546C67;
enum chunkType : uint32_t {
  JSON = 0x4E4F534A,
  BIN = 0x004E4942,
};

std::string formatJSON(const std::vector<unsigned char> &v) {
  std::string result = "";
  result.reserve(v.size() * 4);
  int indentation = 0;

  for (const unsigned char &uc : v) {
    switch (uc) {
    case '{':
      result += "{\n";
      result.append(++indentation * 4, ' ');
      break;
    case '}':
      result += '\n';
      result.append(--indentation * 4, ' ');
      result += "}\n";
      result.append(indentation * 4, ' ');
      break;
    case '[':
      result += "[\n";
      result.append(++indentation * 4, ' ');
      break;
    case ']':
      result += '\n';
      result.append(--indentation * 4, ' ');
      result += "]\n";
      result.append(indentation * 4, ' ');
      break;
    case ',':
      result += ",\n";
      result.append(indentation * 4, ' ');
      break;
    default:
      result += uc;
      break;
    }
  }

  return result;
}

class glTFchunk {
public:
  uint32_t type;
  std::vector<unsigned char> data;

  glTFchunk(uint32_t type, std::vector<unsigned char> &&chunkData) {
    this->type = type;
    this->data = std::move(chunkData);
  }

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

  glTFmodel(const std::string &fileName) {
    this->chunks = {};
    std::ifstream file(fileName, std::ios::binary | std::ios::in);

    if (!file || !file.is_open() || file.fail()) {
      std::print("!!!Couldn't open/find file: {}\n", fileName);
      this->magic = 0;
      this->version = 0;
      this->length = 0;
      return;
    }

    file.seekg(0);
    uint32_t header[3];
    file.read(reinterpret_cast<char *>(&header), sizeof(header));

    if (header[0] != glTFmagicValue) {
      std::print("!!!File not .glb (found: {}, expected: 0x{:08X})\n",
                 header[0], glTFmagicValue);
      this->magic = 0;
      this->version = 0;
      this->length = 0;
      return;
    }

    this->magic = header[0];
    this->version = header[1];
    this->length = header[2];
    this->chunks.reserve(2);

    uint32_t size = header[2] - sizeof(header);
    while (size) {
      uint32_t data[2];

      file.read(reinterpret_cast<char *>(&data), sizeof(data));
      glTFchunk newChunk(data[1], std::vector<unsigned char>(data[0]));
      file.read(reinterpret_cast<char *>(newChunk.data.data()), data[0]);

      this->chunks.push_back(newChunk);

      size -= sizeof(data) + data[0];
    }

    file.close();
  }

  void info() {
    std::print("====model====\nmagic: 0x{:08X}\nversion: {}\nlength: {}\n",
               this->magic, this->version, this->length);

    for (size_t i = 0; const glTFchunk &chunk : this->chunks) {
      std::print("----chunk {}----\n\ttype: {}\n\tdata:\n", i++,
                 glTFchunk::typeToString(chunk.type));

      if (chunk.type == chunkType::JSON) {
        std::print("{}\n", formatJSON(chunk.data));
      }
    }
  }
};
} // namespace fileHandler

int main() {
  fileHandler::glTFmodel model("./models/cube.glb");
  model.info();
}
