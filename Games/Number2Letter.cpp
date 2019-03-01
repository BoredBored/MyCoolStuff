#include <iostream>
#include <string>

int main() {
  int letter = 0;
  std::cout << "Please type in a number and a letter will be printed on screen corresponding to it's value in the English Alphabet" << std::endl << std::endl;
  std::string StarterString("The letter that corosponds to that value is ");
  std::string DamDatLetter;

  bool loopy = true;
  while (loopy) {
    std::cin >> letter;
    if (letter > 26 || letter < 1) {
      std::cout <<\
"The English Alphabet only has 26 letters.\n\
Try again\n";
    } else {
      DamDatLetter += 64 + letter;
      loopy = false;
    }
  }
  std::cout << std::endl << StarterString << '\'' << DamDatLetter << "\'\n";
}
