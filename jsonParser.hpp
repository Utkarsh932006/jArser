// This is a header file for the jsonParser
// Created by Utkarsh 31-JULY-2026

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <iterator>

union jsonValue
{
  int i;
  double d;
  std::map<std::string, jsonValue>* json;
};

void fileReader(std::string filePath, std::string& output);

namespace jsonParser
{
jsonValue parsePrimitive(const std::string&, std::string::iterator,
                         std::string::iterator);
jsonValue parseJsonHelper(const std::string&, std::string::iterator);
}
