#include <iostream>
#ifdef __cplusplus
extern "C"
{
#endif
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#ifdef __cplusplus
}
#endif
#include <cctype>

using namespace std;

bool isinteger(std::string const& n) noexcept
{
    if (std::isdigit(n[0]) || (n.size() > 1 && (n[0] == '-' || n[0] == '+')))
    {
        for (std::string::size_type i{ 1 }; i < n.size(); ++i)
            if (!std::isdigit(n[i]))
                return false;

        return true;
    }
    return false;
}

std::string multiplyStr(int num, std::string& str){
	std::string stry = "";
	for(int i=0; i < num; i++){
		stry += str;
	}
	return stry;
}
void help(std::string& pname){
	cout<<"usage: "<< pname <<" [string] [num of the string]\n";
}
int main(int argc, char *argv[] ){
	if ( argc != 3 ){
		help(argv[0]);
	} else if(!isinteger(argv[2])){
		help(argv[0]);
	} else {
		int indent = std::stoi( argv[2] );
		std::cout << multiplyStr(indent, argv[1])+'\n';
	}
	return 0;
}
