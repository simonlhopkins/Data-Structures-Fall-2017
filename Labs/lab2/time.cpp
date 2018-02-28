#include <iostream>

#include <iomanip>
#include <string>
#include "time.h"

Time::Time(){
	hour=0;
	minute=0;
	second=0;
}

Time::Time(int tHour, int tMinute, int tSecond){
	hour=tHour;
	minute=tMinute;
	second=tSecond;
}

int Time::getHour() const{
	return hour;
}
int Time::getMinute() const{
	return minute;
}
int Time::getSecond() const{
	return second;
}
void Time::setHour(int newHour){
	hour=newHour;
}
void Time::setMinute(int newMinute){
	minute=newMinute;
}
void Time::setSecond(int newSecond){
	second=newSecond;
}
void Time::PrintAMPM(){
	if(hour<=12){
		//AM
		if(hour==0){
			std::cout<<"12"<<":"<<std::setfill('0') << std::setw(2) <<minute<<":"<<std::setfill('0') << std::setw(2) <<second;

		}
		else{
			std::cout<<hour<<":"<<std::setfill('0') << std::setw(2) <<minute<<":"<<std::setfill('0') << std::setw(2) <<second;
		}
	}
	else{
		//PM
		std::cout<<hour-12<<":"<<std::setfill('0') << std::setw(2) <<minute<<":"<<std::setfill('0') << std::setw(2) <<second;
	}
	if(hour<12){
		std::cout<<" AM"<< std::endl;
	}
	else{
		std::cout<<" PM"<< std::endl;
	}
	
}
//return true if first is bigger than second
bool IsEarlierThan(const Time& t1, const Time& t2){
	if(t1.getHour()>t2.getHour()){
		return true;
	}
	else{
		if(t1.getHour()==t2.getHour()){
			if(t1.getMinute()>t2.getMinute()){
				return true;
			}
			else{
				if(t1.getMinute()==t2.getMinute()){
					if(t1.getSecond()>t2.getSecond()){
						return true;
					}
					else{
						return false;
					}
				}
				return false;
			}
		}
		return false;
	}


}


