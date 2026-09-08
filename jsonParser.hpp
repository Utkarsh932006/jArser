// This is a header file for the jsonParser
// Created by Utkarsh 31-JULY-2026

#include <map>
#include <optional>
#include <string>
#include <variant>
#include <vector>

struct jsonValue;
using jsonObject = std::map<std::string, jsonValue>;
using jsonArray = std::vector<jsonValue>;

using jsonData = std::variant<std::nullptr_t,
                              bool,
                              int64_t,
                              double,
                              std::string,
                              jsonArray,
                              jsonObject>;

struct jsonValue
{
  jsonData data;
};

std::string
readFile(const std::string& filePath);

namespace jsonParser {
std::optional<jsonValue> parsePrimitive(std::string::iterator,
                                        std::string::iterator);

std::optional<jsonValue>
parseJsonHelper(const std::string&, std::string::iterator&);

std::optional<std::pair<std::string, jsonValue>>
retrieveKeyValuePair(const std::string&, std::string::iterator&);

std::optional<jsonValue>
parseJson(const std::string&);

void
printJson(const jsonValue&, int indent = 0);
}
