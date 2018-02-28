#include <iostream>
#include <map>

int main(){

	// std::map<int, int> occurances;
	// for(int i=0; i<11; i++){
	// 	int newNum;
	// 	std::cin>>newNum;
	// 	++occurances[newNum];
	// }
	// std::map<int, int>::const_iterator itr;
	// int max=0;
	// for (itr = occurances.begin(); itr != occurances.end(); ++itr) {
	// 	if((itr->second)>max){
	// 		max=itr->second;
	// 	}
	// }
	// for (itr = occurances.begin(); itr != occurances.end(); ++itr) {
	// 	if((itr->second)==max){
	// 		std::cout<<itr->first<<"\t"<<itr->second<<std::endl;
	// 	}
	// }
	std::map<int, int> occurances;
	for(int i=0; i<11; i++){
		int newNum;
		std::cin>>newNum;
		std::map<int,int>::iterator itr;
		itr= occurances.find(newNum);
		if(itr != occurances.end()){
			//it exsists in the list
			itr->second+=1;
		}
		else{
			occurances.insert(std::pair<int,int>(newNum,1));

		}
	}
	std::map<int, int>::const_iterator itr;
	int max=0;
	for (itr = occurances.begin(); itr != occurances.end(); ++itr) {
		if((itr->second)>max){
			max=itr->second;
		}
	}
	for (itr = occurances.begin(); itr != occurances.end(); ++itr) {
		if((itr->second)==max){
			std::cout<<itr->first<<"\t"<<itr->second<<std::endl;
		}
	}


	return 0;
}