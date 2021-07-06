#include "include/helpers.h"

#include <string>

namespace ag {
  char upcase(char character) {
    char upcased_char;

    if (character >= 97 && character <= 122)
      upcased_char = character - 32;
    else
      upcased_char = character;

    return upcased_char;
  }

  char downcase(char character) {
    char downcased_char;

    if (character >= 65 && character <= 90)
      downcased_char = character + 32;
    else
      downcased_char = character;

    return downcased_char;
  }

  std::string capitalize(std::string str) {
    std::string new_str;
    new_str += upcase(str[0]);

    for (int i = 1; i < str.size(); i++) {
      char next_char = str[i];

      if (next_char >= 65 && next_char <= 90)
        new_str += (next_char + 32);
      else
        new_str += next_char;
    }

    return new_str;
  }
}
