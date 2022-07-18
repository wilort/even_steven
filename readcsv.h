#pragma once
#include <vector>

void trimLeft(std::string& s, const std::string& trimChars);
void trimRigght(std::string& s, const std::string& trimChars);
void trim(std::string& s, const std::string& trimChars);
std::vector<std::string> readLine(std::string csvLine);
std::vector<std::vector<std::string> > readLines(std::ifstream& csvFile);
std::vector<std::vector<std::string> > readFile(const std::string fileName);
