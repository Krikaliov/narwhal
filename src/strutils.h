#ifndef STRUTILS_H
#define STRUTILS_H 1

#include <vector>
#include <string>

namespace N
{
  /// @brief Convert a string to a char list (vector)
  /// @param s original string
  /// @return a char list (vector).
  std::vector<char> stringToCharList(std::string s);

  /// @brief Convert a char list (vector) to a string
  /// @param v original char list (vector)
  /// @return a string.
  std::string charListToString(std::vector<char> v);

  /// @brief Print an integer into a string
  /// @param i integer to print
  /// @return string with the printed integer.
  std::string intToString(int i);
}

#endif // STRUTILS_H
