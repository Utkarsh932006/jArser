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

  if (floatPointIndex >= end - start)
    return { .i = std::stoi(substr) };
  else
    return { .d = std::stod(substr) };
}