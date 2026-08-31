// This is a Json Parser
// Created by Utkarsh 31-JULY-2026

#include "jsonParser.hpp"

int
main()
{
  std::string output;

  fileReader("jsonExample.json", output);
  std::cout << output << "\n";

  return 0;
}

void
fileReader(std::string filePath, std::string& output)
{
  std::ifstream file(filePath);
  std::string inputBuffer;

  while (std::getline(file, inputBuffer))
    {
      output.append(inputBuffer);
    }
}

jsonValue
jsonParser::parsePrimitive(const std::string& output,
                           std::string::iterator start,
                           std::string::iterator end)
{
  std::string substr = output.substr(start - output.begin(), end - start);
  size_t floatPointIndex = substr.find(".");

  if ((int)floatPointIndex >= end - start)
    return { .i = std::stoi(substr) };
  else
    return { .d = std::stod(substr) };
}

std::pair<std::string, jsonValue>
jsonParser::retrieveKeyValuePair(const std::string& output,
                                 std::string::iterator& inputBuffer)
{
  assert(inputBuffer != output.end());

  while (*inputBuffer == ' ' || *inputBuffer == '\n')
    {
      inputBuffer++;
    }

  std::string::iterator currPos = inputBuffer;
  std::string key;
  jsonValue value{};

  if (*inputBuffer == '\"')
    {
      currPos++;
      while (*inputBuffer != '\"')
        {
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

  std::map<std::string, jsonValue>* jsonMap
      = new std::map<std::string, jsonValue>;
  do
    {
      const auto [key, value] = retrieveKeyValuePair(output, inputBuffer);
      (*jsonMap)[key] = value;

      while (*inputBuffer == ' ' || *inputBuffer == '\n')
        {
          inputBuffer++;
        }
    }
  while (*inputBuffer != '}');
  inputBuffer++;
  return { .json = jsonMap };
}

jsonValue
jsonParser::parseJson(const std::string& filepath)
{
  std::string text;
  fileReader(filepath, text);

  std::string::iterator start = text.begin();
  return parseJsonHelper(text, start);
}
