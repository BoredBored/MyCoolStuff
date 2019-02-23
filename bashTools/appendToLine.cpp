#include <sstream>
#include <iostream>

void help(std::string& pname){
	std::cout<<"usage: "<< pname <<" [string] [string to add to each line]\n";
}

int main(int argc, char *argv[] ){
	if ( argc != 3 ){
		help(argv[0]);
	} else {
		std::istringstream f(argv[1]);
		std::string line;
		std::string res;
		while (std::getline(f, line)) {
			res +=	argv[2] + line +'\n';
		}
		std::cout << res;
	}
	return 0;
}
