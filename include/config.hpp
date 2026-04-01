#include <string>

namespace config {
const std::string applicationName = "MineCloneCraft";
const int majoranta = 0, minoranta = 0, patch = 1;

inline std::string fullName() {
  return applicationName + '-' + std::to_string(majoranta) + '.' + std::to_string(minoranta) + '.' + std::to_string(patch);
}

inline std::string engineName() {
  return applicationName + "-Engine";
}
};
