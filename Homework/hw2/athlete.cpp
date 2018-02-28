#include <string>
#include <iostream>
#include "athlete.h"

Athlete::Athlete(std::string pFName, std::string pLName, std::string pCountry, std::vector<Event> pAllEventVector){
	fName=pFName;
	lName=pLName;
	country=pCountry;
	totalPoints=0;
	totalDistance=0;
	totalTime=0;
	//copies the components of argument vector to member vector
	for (int i=0; i<pAllEventVector.size(); i++){
        allEventVector.push_back(pAllEventVector[i]);
    }
}

//get the full name of the athlete
std::string Athlete::getFullName() const{
	return(fName+" "+lName);
}

std::string Athlete::getCountry() const{
	return(country);
}
std::string Athlete::getFirstName() const{
	return fName;
}
std::string Athlete::getLastName() const{
	return lName;
}

//this function replaces an event in allevent vector by replacing it with the same event type but with a score
void Athlete::addEvent(Event newEvent){
	for(int i=0; i< allEventVector.size();i++){
		if(newEvent.getName()==allEventVector[i].getName()){
			allEventVector[i]=newEvent;
		}
	}
	std::string score= newEvent.getScore();

	//adds to totalpoints when you add a new event
	totalPoints+=newEvent.calcPoints();
	
}

//returns all of the events
std::vector<Event> Athlete::getEventsParticipated(){
	return allEventVector;
}
float Athlete::getTotalPoints() const{
	return totalPoints;
}
void Athlete::addToTotalTime(float newTime){
	totalTime+=newTime;
}
float Athlete::getTotalTime() const{
	return totalTime;
}

//adds a normalized value to totalDistance
void Athlete::addToTotalDistance(float newDistance, float scaler){
	totalDistance+=newDistance/scaler;
}

//returns the total distance
float Athlete::getTotalDistance() const{
	if(totalDistance<0.001){
		return 0;
	}
	return totalDistance;
}

//compare functions used for std::sort for eac different data type
bool scoreCompare (const Athlete& a, const Athlete& b){
	if(a.getCountry()!=b.getCountry()){
		return a.getCountry()<b.getCountry();
	}
	else{
		return a.getLastName()<b.getLastName();
	}
}

bool pointCompare (const Athlete& a, const Athlete& b){
	if(a.getTotalPoints()!=b.getTotalPoints()){
		return a.getTotalPoints()>b.getTotalPoints();
	}
	else{
		return a.getLastName()<b.getLastName();
	}
}

bool customCompare (const Athlete& a, const Athlete& b){
	if(a.getTotalDistance()!=b.getTotalDistance()){
		return a.getTotalDistance()>b.getTotalDistance();
	}
	else{
		return a.getLastName()<b.getLastName();
	}
}




