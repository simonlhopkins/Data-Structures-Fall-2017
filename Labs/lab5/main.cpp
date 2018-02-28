#include <vector>
#include <iostream>
#include <list>

void reverse_vector(std::vector<int> &inputVec){

	for(int i=0; i<inputVec.size()/2; ++i){
		int tempBig=inputVec[inputVec.size()-(i+1)];
		int tempSmall=inputVec[i];
		inputVec[i]=tempBig;
		inputVec[inputVec.size()-(i+1)]=tempSmall;
	}
}

void reverse_list(std::list<int> &inputlist){
	std::list<int>::iterator beginItr;
	std::list<int>::reverse_iterator endItr;
	beginItr= inputlist.begin();
	endItr= inputlist.rbegin();
	int counter=0;

	while(counter<(inputlist.size()/2)){
		int temp= *beginItr;
		*beginItr=*endItr;
		*endItr=temp;


		++beginItr;
		++endItr;
		++counter;
	}
	std::list<int>::iterator tempItr;

	for(tempItr=inputlist.begin(); tempItr!=inputlist.end(); ++tempItr){
		std::cout<<*tempItr<<", ";
	}
	std::cout<<std::endl;
}


int main(){
	std::vector<int> v;
	std::list<int> l;
	l.push_back(1);
	l.push_back(7);
	l.push_back(5);
	l.push_back(2);
	l.push_back(6);
	l.push_back(0);
	reverse_list(l);
	reverse_vector(v);
	
	
	return 0;
}