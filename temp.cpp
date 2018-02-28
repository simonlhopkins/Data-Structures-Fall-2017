#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <set>
#include <list>

void checkIfNumisNegative(int x){
	if(x<0){
		throw(std::string("ERROR... number is negative"));
	}
}
template <class T>
class Example1{
public:
	//default contructor
	Example1<T>(const std::vector<int> posNumbers_, const std::string name_, const T special_)
	:posNumbers(posNumbers_), name(name_), special(special_){
		for(int i=0; i< posNumbers.size(); i++){
			try{
				checkIfNumisNegative(posNumbers[i]);
			}
			catch(std::string warning){
				std::cout<<warning<<std::endl;
			}
		}
	}
	//copy constructor
	Example1<T>(const Example1 &e2) {
		posNumbers = e2.posNumbers;
		name = e2.name;
		special= e2.special;
	}
	//assignment constructor
	Example1<T> &operator=(const Example1 &e2){
		if(e2==this){
			return *this;
		}else{
			Example1(e2);
			return *this;
		}
	}
	//less than overload
	bool operator<(const Example1 &e2){
		return(this->special<e2.special);
	}

	~Example1(){}


	virtual const std::string getName() const{ return name+"_example1"; }
	const T getSpecial() const{
		return special;
	}

protected:
	std::string name;
	std::vector<int> posNumbers;
	T special;
};

template <class T>
bool operator==(const Example1<T> e1, const Example1<T> e2){
	return(e1.getSpecial()==e2.getSpecial());
}

template <class T>
class Example2: public Example1<T> {
public:
	Example2(const std::vector<int> posNumbers_, const std::string name_, const T special_)
	: Example1<T>(posNumbers_, name_, special_){}

	std::string getName(){return this->name+ "_example2"; }

};



int main(){
	std::vector<int> v;
	for(int i=0; i< 10; i++){
		v.push_back(i);
	}
	Example1<char> ex1= Example1<char>(v, "Simon", 'x');
	Example1<char> ex2 = ex1;
	std::cout<<(ex1==ex2)<<std::endl;
	Example2<std::string> ex3= Example2<std::string>(v, "Simon", "xxx");
	Example2<char>* ex4= new Example2<char>(v, "Igor", 'e');
	std::cout<<ex2.getName()<<std::endl;
	std::cout<<ex3.getName()<<std::endl;
	//casting an Example2 class as Example1
	std::cout<<ex4->getName()<<std::endl;
	Example1<char>* exPointer= dynamic_cast<Example1<char>*>(ex4);
	std::cout<<exPointer->getName()<<std::endl;

	std::map<std::string, std::vector<int> > exMap;
	std::vector<int> v1;
	for(int i=0; i<4; i++){v1.push_back(i);}
	exMap["Troy"] = v1;
	std::vector<int> v2;
	for(int i=0; i<10; i+=2){v2.push_back(i);}
	exMap.insert(std::pair<std::string, std::vector<int> >("Abed", v2));
	for(std::map<std::string, std::vector<int> >::const_iterator itr=exMap.begin(); itr!=exMap.end(); itr++){
		std::cout<<itr->first<<std::endl;
		for(std::vector<int>::const_iterator itr2= itr->second.begin(); itr2!= itr->second.end(); itr2++){
			std::cout<<"\t"<<*itr2<<std::endl;
		}
	}
	int *b= new int;
	std::cout<<b<<std::endl;
	b++;
	std::cout<<b<<std::endl;

	std::list<int> l1;
	for(int i=0; i<10; i++){l1.push_back(i);}
	for(std::list<int>::const_iterator itr=l1.begin(); itr!=l1.end(); itr++){
		if(*itr%2==0){
			itr=l1.erase(itr);
		}
	}
	for(std::list<int>::const_iterator itr=l1.begin(); itr!=l1.end(); itr++){
		std::cout<<*itr<<std::endl;
	}

	std::pair<int, std::string> test = std::pair<int, std::string>(4, "Jeff");
	std::pair<int, std::string> test2= std::make_pair(4, "Jeff");
	std::cout<<(test==test2)<<std::endl;



	return 0;
}
/*
1
Simon_example1
Simon_example2
Igor_example2
Igor_example1

Abed
	0
	2
	4
	6
	8
Troy
	0
	1
	2
	3
*/

