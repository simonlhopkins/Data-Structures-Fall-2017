#include <iostream>
#include <cmath>


int findPath(int startX,int startY){if(startX<0||startY<0){return 0;}if(startX==0||startY==0){return 1;}return(findPath(startX-1,startY)+findPath(startX,startY-1));}int main(){int startX,startY;std::cin>>startX>>startY;std::cout<<findPath(startX,startY)<<std::endl;return 0;}




