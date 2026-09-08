// This is a Json Parser
// Created by Utkarsh 31-JULY-2026

#include "jsonParser.hpp"
#include <cassert>
#include <charconv>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>

int
main(int argc, char* argv[])
{
  if (argc != 2)
    return EXIT_FAILURE;

  std::string output;
  readFile(argv[1]);
  std::cout << output << "\n";

  return EXIT_SUCCESS;
}

std::string
readFile(const std::string& filePath)
{
  std::ifstream file(filePath, std::ios::in | std::ios::binary);
  std::ostringstream inputBuffer;
  inputBuffer << file.rdbuf();
  return inputBuffer.str();
}

std::optional<jsonValue>
jsonParser::parsePrimitive(std::string::iterator start,
                           std::string::iterator end)
{
  std::string_view substr(start, end);
  size_t floatPointIndex = substr.find('.');

  if (floatPointIndex == std::string_view::npos) {
    int value{ 0 };
    auto [ptr, ec] =
      std::from_chars(substr.data(), substr.data() + substr.size(), value);

    if (ec != std::errc())
      return std::nullopt;
    else
      return jsonValue{ .data = value };

  } else {
    double value{ 0.0 };
    auto [ptr, ec] =
      std::from_chars(substr.data(), substr.data() + substr.size(), value);

    if (ec != std::errc())
      return std::nullopt;
    else
      return jsonValue{ .data = value };
  }
}

std::optional<std::pair<std::string, jsonValue>>
jsonParser::retrieveKeyValuePair(const std::string& output,
                                 std::string::iterator& inputBuffer)
{
  auto skipWhitespace = [&]() {
    while (inputBuffer != output.end() &&
           (*inputBuffer == ' ' || *inputBuffer == '\n' ||
            *inputBuffer == '\t' || *inputBuffer == '\r')) {
      ++inputBuffer;
    }
  };

  skipWhitespace();

  if (inputBuffer == output.end() || *inputBuffer != '\"')
    return std::nullopt;
  ++inputBuffer;

  auto keyStart = inputBuffer;
  while (inputBuffer != output.end() && *inputBuffer != '\"') {
    ++inputBuffer;
  }

  if (inputBuffer == output.end())
    return std::nullopt;

  std::string key(keyStart, inputBuffer);
  ++inputBuffer;

  skipWhitespace();

  if (inputBuffer == output.end() || *inputBuffer != ':')
    return std::nullopt;
  ++inputBuffer;

  skipWhitespace();

  std::optional<jsonValue> val;
  if (inputBuffer != output.end() && *inputBuffer == '{') {
    val = parseJsonHelper(output, inputBuffer);
  } else {
    auto valStart = inputBuffer;
    while (inputBuffer != output.end() && *inputBuffer != ',' &&
           *inputBuffer != '}' && *inputBuffer != '\n') {
      ++inputBuffer;
    }
    val = parsePrimitive(valStart, inputBuffer);
  }

  if (!val)
    return std::nullopt;

  return std::make_pair(std::move(key), std::move(*val));
}

std::optional<jsonValue>
jsonParser::parseJsonHelper(const std::string& output,
                            std::string::iterator& inputBuffer)
{
  if (inputBuffer == output.end() || *inputBuffer != '{')
    return std::nullopt;
  inputBuffer++;

  jsonObject jsonMap{};

  while (inputBuffer != output.end() && *inputBuffer != '}') {
    while (inputBuffer != output.end() &&
           (*inputBuffer == ' ' || *inputBuffer == '\n')) {
      ++inputBuffer;
    }
    if (inputBuffer != output.end() && *inputBuffer == '}')
      break;

    auto kv = retrieveKeyValuePair(output, inputBuffer);
    if (!kv)
      return std::nullopt;

    auto [key, val] = std::move(*kv);
    jsonMap.emplace(std::move(key), std::move(val));

    while (inputBuffer != output.end() &&
           (*inputBuffer == ' ' || *inputBuffer == '\n')) {
      ++inputBuffer;
    }
    if (inputBuffer != output.end() && *inputBuffer == ',')
      ++inputBuffer;
  }

  if (inputBuffer == output.end() || *inputBuffer != '}')
    return std::nullopt;
  ++inputBuffer;

  return jsonValue{ .data = std::move(jsonMap) };
}

std::optional<jsonValue>
jsonParser::parseJson(const std::string& filepath)
{
  std::string text;
  readFile(filepath);

  std::string::iterator start = text.begin();
  return parseJsonHelper(text, start);
}

void
jsonParser::printJson(const jsonValue& val, int indent)
{
  std::string pad(static_cast<size_t>(indent * 2), ' ');
  std::visit(
    [&](const auto& arg) {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T, std::nullptr_t>) {
        std::cout << "null";
      } else if constexpr (std::is_same_v<T, bool>) {
        std::cout << (arg ? "true" : "false");
      } else if constexpr (std::is_same_v<T, int64_t> || std::is_same_v<T, double>) {
        std::cout << arg;
      } else if constexpr (std::is_same_v<T, std::string>) {
        std::cout << '\"' << arg << '\"';
      } else if constexpr (std::is_same_v<T, jsonArray>) {
        if (arg.empty()) {
          std::cout << "[]";
          return;
        }
        std::cout << "[\n";
        for (size_t i = 0; i < arg.size(); ++i) {
          std::cout << pad << "  ";
          printJson(arg[i], indent + 1);
          if (i + 1 < arg.size())
            std::cout << ",";
          std::cout << "\n";
        }
        std::cout << pad << "]";
      } else if constexpr (std::is_same_v<T, jsonObject>) {
        if (arg.empty()) {
          std::cout << "{}";
          return;
        }
        std::cout << "{\n";
        size_t count = 0;
        for (const auto& [k, v] : arg) {
          std::cout << pad << "  \"" << k << "\": ";
          printJson(v, indent + 1);
          if (++count < arg.size())
            std::cout << ",";
          std::cout << "\n";
        }
        std::cout << pad << "}";
      }
    },
    val.data);
}

