#include <iostream>
#include <cmath>
#include <string>
#include <math.h>
#include <iomanip>
#include <vector>



// void print_square(std::string wordToPrint){
// 	int sideLength= std::sqrt(wordToPrint.length())+1;
// 	for(int i =0; i< sideLength+2; i++){
// 		std::cout<<"*";
// 	}
// 	std::cout<<std::endl;

// 	for(int i = 0; i< (sideLength); i++){
// 		std::cout<<"*";
// 		if(i*sideLength + sideLength >= wordToPrint.length()){
// 			std::cout<<std::setfill(' ')<<std::setw(sideLength)<<std::left<<wordToPrint.substr(int(i*sideLength)-1, int(wordToPrint.length())-int(i*sideLength));

// 			std::cout<<"*"<<std::endl;
// 			continue;
// 		}
// 		std::cout<<std::setfill(' ')<<std::setw(sideLength)<<std::left<<wordToPrint.substr(i*sideLength, sideLength);
// 		std::cout<<"*"<<std::endl;
// 	}
// 	for(int i =0; i< sideLength+2; i++){
// 		std::cout<<"*";
// 	}
// 	std::cout<<std::endl;
// }

// int main(){
// 	print_square("Here is an example.");
// print_square("the quick brown fox jumped over the lazy dogs");
// print_square("Twinkle, twinkle, little star, how I wonder what you are. Up above the world so high, like a diamond in the sky.");

// 	return 0;
// }

void HasLetter(std::vector<std::string> &words, char letter, std::vector<std::string> &selected){
	for(int i=0; i<words.size();i++){
		if(words[i].find(letter)<words[i].length()){
			selected.push_back(words[i]);
		}
	}
}

int main(){

	// std::vector<std::string> v1;
	// v1.push_back("I");
	// v1.push_back("wish");
	// v1.push_back("you");
	// v1.push_back("would");
	// v1.push_back("step");
	// v1.push_back("back");
	// v1.push_back("from");
	// v1.push_back("that");
	// v1.push_back("ledge");
	// v1.push_back("my");
	// v1.push_back("friend");
	// std::vector<std::string> v2;
	// HasLetter(v1, 'u', v2);
	// for(int i=0; i<v2.size();i++){

	// 	std::cout<<v2[i]<<std::endl;
	// }
	char *cat; //regular pointer (stores a memory location)
	char **dog; //pointer to a pointer
	char fish[2]; //makes array of size 2
	char horse; //makes char
	dog= new char *[3]; //sets dog to an array of pointers on the heap of size 3
	dog[0] = new char; //sets the 0th index of dog in the heap to a new char
	fish[0]= 'b';
	fish[1]= 'i';
	dog[1]= &fish[1]; //dog 1 is equal to the memory location of the first index of fish
	dog[2]= &horse; //dog 2 is equal to the memory location of horse
	cat= dog[0]; //sets cat equal to the value at dog 0, which rn is unitialized
	*cat= 'r'; //sets dog[0] equal to r, dog[0] is a pointer to a new char on the heap
	horse='d';
	std::cout<<*cat<<std::endl;

}