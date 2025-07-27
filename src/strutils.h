#ifndef STRUTILS_H
#define STRUTILS_H 1

#include <vector>
#include <string>

std::vector<char> str_to_vecchar(std::string s);
std::string vecchar_to_str(std::vector<char> v);
std::string float_to_string(float f);
std::string int_to_string(int i);

#endif // STRUTILS_H
