#ifndef ENV_PARSER_H
#define ENV_PARSER_H

#include <string>
#include <map>
#include <fstream>
#include <sstream>

class EnvParser {
private:
    std::map<std::string, std::string> envMap;

    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\r\n\"");
        if (std::string::npos == first) {
            return "";
        }
        size_t last = str.find_last_not_of(" \t\r\n\"");
        return str.substr(first, (last - first + 1));
    }

public:
    EnvParser() = default;

    explicit EnvParser(const std::string& filePath) {
        load(filePath);
    }

    bool load(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            return false;
        }
        std::string line;
        while (std::getline(file, line)) {
            size_t firstNonSpace = line.find_first_not_of(" \t");
            if (firstNonSpace == std::string::npos || line[firstNonSpace] == '#') {
                continue; 
            }
            size_t equalPos = line.find('=');
            if (equalPos == std::string::npos) {
                continue;
            }
            std::string key = trim(line.substr(0, equalPos));
            std::string val = trim(line.substr(equalPos + 1));
            envMap[key] = val;
        }
        return true;
    }

    std::string getValue(const std::string& key, const std::string& defaultValue = "") const {
        auto it = envMap.find(key);
        if (it != envMap.end()) {
            return it->second;
        }
        return defaultValue;
    }
};

#endif // ENV_PARSER_H
