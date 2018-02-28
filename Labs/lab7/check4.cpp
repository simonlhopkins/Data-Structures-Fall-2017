
#include <list>
#include <iostream>
void reverse_splice(std::list<int>::iterator begin, std::list<int>::iterator end, std::list<int> &l){
	while(begin!=end){
		std::list<int>::iterator itr;
		itr=l.begin();
		while(itr!=l.end()){
			std::cout<<*itr<<", ";
			itr++;
		}
		std::cout<<std::endl;
		end = l.insert(end, *begin);
		begin = l.erase(begin);
	}
	
}

int main(){
	std::list<int> myList;
	myList.push_back(1);
	myList.push_back(2);
	myList.push_back(3);
	myList.push_back(4);
	myList.push_back(5);
	myList.push_back(6);
	myList.push_back(7);
	myList.push_back(8);
	myList.push_back(9);

	std::list<int>::iterator begin;
	std::list<int>::iterator end;

	begin=myList.begin();
	begin++;
	begin++;
	begin++;
	end= myList.begin();
	end++;
	end++;
	end++;
	end++;
	end++;
	end++;
	end++;

	reverse_splice(begin, end, myList);
	return 0;
}