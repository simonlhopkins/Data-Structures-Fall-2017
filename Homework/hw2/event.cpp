#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include "event.h"


//constructor
Event::Event(std::string eName, float eA, float eB, float eC, bool eIsTrack, 
	float eScale, std::string eAbbreviation){
	name=eName;
	A=eA;
	B=eB;
	C=eC;
	isTrack=eIsTrack;
	score="0";
	scale=eScale;
	abbreviation=eAbbreviation;
}

std::string Event::getName() const{
	return name;
}

bool Event::getIsTrack() const{
	return isTrack;
}

void Event::setPoints(int newPoints){
	points=newPoints;
}

void Event::setScore(std::string newScore){
	score=newScore;
	
}

std::string Event::getScore() const{
	return score;
}

//calculates the point value by first checking if it is a track or field event, and then applying the equation
int Event::calcPoints(){
	float scoreFloat;
	//specific conversion if it is 1500 meters
	if(name=="1500_METERS"){
		float min= std::stof(score.substr(0, score.find(":")));
		float sec= std::stof(score.substr(score.find(":")+1));

		scoreFloat=((60*min)+(sec));
	}
	else{
		scoreFloat=std::stof(score);
	}

	//return 0 (don't bother with the equation) if the score is 0
	if(score=="0"){
		return 0;
	}
	//computes for track or field
	if(isTrack){
		return(int(A* std::pow(B-float(scale*scoreFloat),C)));
	}
	else{
		return(int(A* std::pow(float(scale*scoreFloat)-B,C)));

	}
	

}
void Event::addToEventTotal(float numToAdd){
	eventTotal+=numToAdd;
}

std::string Event::getAbbreviation() const{
	return abbreviation;
}
float Event::getTotalPoints() const{
	return eventTotal;
}

float Event::getScale() const{
	return scale;
}



bool operator== (const Event& a, const Event& b){
	if(a.getName()==b.getName()){
		return true;
	}else{
		return false;
	}
}





