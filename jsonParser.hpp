// This is a header file for the jsonParser
// Created by Utkarsh 31-JULY-2026

#include <map>
#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>

union jsonValue
{
  std::int64_t i;
  double d;
  std::map<std::string, jsonValue>* json;
};

void fileReader(std::string filePath, std::string& output);

int filter();

jsonValue jsonParser(const std::string&, std::string::iterator,
                     std::string::iterator);