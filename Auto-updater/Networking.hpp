#pragma once
#include <string>

std::string getRequest(const char* url);
std::string downloadFile(const char* url, const char* path);
std::string extractData(std::string data, std::string variable);
const char* toChar(const std::string& str);
std::string removeDoubleQuotes(const std::string& input);
