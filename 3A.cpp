#include <iostream>
#include <string>

int main() {
int b(0);
std::string letterletter;
std::string MistakeMessage1("\"I'm sorry, but I think the English Alphabet only has 26 letters.\"");
std::string MistakeMessage2("\"Try again -- but this time lets see if you're a bit smarter\"");
std::cout << 
"#include <iostream>" << std::endl <<
"#include <string>" << std::endl <<
std::endl <<
"int main() {" << std::endl <<
"int letter=0;" << std::endl << 
"std::cout << \"Please type in a number and a letter will be printed on screen corresponding to it's value in the English Alphabet\" << " << "std::endl << std::endl;" << std::endl <<
"std::string StarterString (\"The letter that corosponds to that value is \");" << std::endl << 
"std::string DamDatLetter;" << std::endl <<
std::endl <<
"Mistake:" << std::endl <<
"std::cin >> letter;" << std::endl;

for( int a = 1; a < 26; a = a + 1 ){
++b;
if(a == 1){letterletter='A';}
if(a == 2){letterletter='B';}
if(a == 3){letterletter='C';}
if(a == 4){letterletter='D';}
if(a == 5){letterletter='E';}
if(a == 6){letterletter='F';}
if(a == 7){letterletter='G';}
if(a == 8){letterletter='H';}
if(a == 9){letterletter='I';}
if(a == 10){letterletter='J';}
if(a == 11){letterletter='K';}
if(a == 12){letterletter='L';}
if(a == 13){letterletter='M';}
if(a == 14){letterletter='N';}
if(a == 15){letterletter='O';}
if(a == 16){letterletter='P';}
if(a == 17){letterletter='Q';}
if(a == 18){letterletter='R';}
if(a == 19){letterletter='S';}
if(a == 20){letterletter='T';}
if(a == 21){letterletter='U';}
if(a == 22){letterletter='V';}
if(a == 23){letterletter='W';}
if(a == 24){letterletter='S';}
if(a == 25){letterletter='Y';}
if(a == 26){letterletter='Z';}
std::cout << "if(letter == " << b << "){DamDatLetter=\"" << letterletter << "\";}" << std::endl;}
std::cout << "if(letter > 26){std::cout <<" << std::endl << MistakeMessage1 << " << std::endl << " << std::endl << MistakeMessage2 << " << std::endl;goto Mistake;}" << std::endl;
std::cout << "std::cout << std::endl << StarterString << DamDatLetter;";
std::cout << std::endl << "}";
}
