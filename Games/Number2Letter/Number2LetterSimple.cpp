#include <iostream>

int main() {
  int letter = 0;
  std::cout << "Please type in a number: ";

  while (1) {
    std::cin >> letter;
    //less than 1 or greater than 26
    if (!(letter < 1 || 26 < letter)) {
        std::cout << "The letter that corosponds to that value is '" << char(64+letter) << "'\n";
      return 0;
    }
    std::cout <<"The English Alphabet only has 26 letters.\nTry again\n";
  }
}
