#include <iostream>

int main(){
	int arrayLength;
	std::cout<<"Enter how many numbers you would like to use: "<<std::endl;
	
	std::cin>>arrayLength;
	int numArray[arrayLength];

	std::cout<<"Enter the numbers"<<std::endl;
	float sum=0;
	for(int i=0;i<arrayLength;i++){
		float temp;
		std::cin>>temp;
		numArray[i]=temp;
		sum+=temp;
	}
	float average= sum/arrayLength;
	std::cout<< "Average is: "<<average<<std::endl;
	std::cout<< "Numbers less than the average "<<std::endl;
	for(int i=0;i<arrayLength;i++){
		if(numArray[i]<average){
			std::cout<<numArray[i]<<std::endl;
		}
	}

}