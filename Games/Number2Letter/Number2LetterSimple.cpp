#include <iostream>

int main() {
  int letter = 0;
  std::cout << "Please type in a number: ";

  while (true) {
    std::cin >> letter;
    //less than 1 or greater than 26
    if (!(letter < 1 || 26 < letter)) {
        std::cout << "The letter that corosponds to that value is '" << char('A' + (letter - 1)) << "'\n";
      return 0;
    }
    std::cout <<"The English Alphabet only has 26 letters.\nTry again\n";
  }
}
