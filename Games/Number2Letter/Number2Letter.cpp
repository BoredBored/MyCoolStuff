#include <iostream>
#include <limits>
#include <string>

int main() {
    int letter = 0;
    const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (true) {
        std::cout << "Please type in a number: ";
        std::cin >> letter;

        if (std::cin.eof()) {
            std::cout << '\n';
            return 1;
        } else if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            std::cout <<"Integer input required. Try again.\n";
        } else if (letter < 1 || letter > 26) {
            std::cout <<"The English alphabet only has 26 letters. Try again.\n";
        } else {
            break;
        }
    }
    std::cout << "The letter that corresponds to that value is '" 
          << alphabet[letter-1] << "'\n";

    return 0;
}
