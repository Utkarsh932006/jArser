// This is a Json Parser
// Created by Utkarsh 31-JULY-2026

#include "jsonParser.hpp"
#include <cassert>
#include <charconv>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>

int
main(int argc, char* argv[])
{
  if (argc != 2)
    return EXIT_FAILURE;

  std::string output;
  readFile(argv[1]);
  std::cout << output << "\n";

  return 0;
}

std::string
readFile(std::string filePath)
{
  std::ifstream file(filePath, std::ios::in | std::ios::binary);
  std::ostringstream inputBuffer;
  inputBuffer << file.rdbuf();
  return inputBuffer.str();
}

jsonValue
jsonParser::parsePrimitive(const std::string& source,
                           std::string::iterator start,
                           std::string::iterator end)
{
  std::string_view substr(&*start, end - start);
  size_t floatPointIndex = substr.find('.');

  if (floatPointIndex == std::string_view::npos) {
    int value{ 0 };
    auto [ptr, ec] =
      std::from_chars(substr.data(), substr.data() + substr.size(), value);

    if (ec != std::errc())
      throw std::invalid_argument("Failed to parse integer primitive value");
    else
      return jsonValue{ .data = value };

  } else {
    double value{ 0.0 };
    auto [ptr, ec] =
      std::from_chars(substr.data(), substr.data() + substr.size(), value);

    if (ec != std::errc())
      throw std::invalid_argument("Failed to parse double primitive value");
    else
      return jsonValue{ .data = value };
  }
}

std::pair<std::string, jsonValue>
jsonParser::retrieveKeyValuePair(const std::string& output,
                                 std::string::iterator& inputBuffer)
{
  assert(inputBuffer != output.end());

  while (*inputBuffer == ' ' || *inputBuffer == '\n') {
    inputBuffer++;
  }

  std::string::iterator currPos = inputBuffer;
  std::string key;
  jsonValue value{};

  if (*inputBuffer == '\"') {
    currPos++;
    while (*inputBuffer != '\"') {
      inputBuffer++;
    }

    key = output.substr(currPos - output.begin(), inputBuffer - currPos);
    assert(*(++inputBuffer) == ':');
    inputBuffer++;
  }

  return std::pair(key, value);
}

jsonValue
jsonParser::parseJsonHelper(const std::string& output,
                            std::string::iterator& inputBuffer)
{
  assert(*inputBuffer == '{');
  inputBuffer++;

  std::map<std::string, jsonValue>* jsonMap =
    new std::map<std::string, jsonValue>;
  do {
    const auto [key, value] = retrieveKeyValuePair(output, inputBuffer);
    (*jsonMap)[key] = value;

    while (*inputBuffer == ' ' || *inputBuffer == '\n') {
      inputBuffer++;
    }
  } while (*inputBuffer != '}');
  inputBuffer++;
  return { .json = jsonMap };
}

jsonValue
jsonParser::parseJson(const std::string& filepath)
{
  std::string text;
  readFile(filepath, text);

  std::string::iterator start = text.begin();
  return parseJsonHelper(text, start);
}
