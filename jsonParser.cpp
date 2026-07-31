// This is a Json Parser
// Created by Utkarsh 31-JULY-2026

#include "jsonParser.hpp"
#include <cassert>

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