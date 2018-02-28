#include <string>
#include <vector>
#ifndef ATHLETE_H
#define ATHLETE_H  //   #define this so the compiler knows it has been included

#include "event.h"

//This the header for the Athlete class, which contains individual information about each athlete on the spreadsheet.


class Athlete{
public:
	Athlete(std::string pFName, std::string pLName, std::string pCountry, std::vector<Event> pAllEventVector);
	//GETTERS

	std::string getFullName() const;
	std::string getFirstName() const;
	std::string getLastName() const;
	std::string getCountry() const;
	float getTotalTime() const;
	float getTotalDistance() const;
	float getTotalPoints() const;
	std::vector<Event> getAllEvents();
	
	//SETTERS
	void addEvent(Event newEvent);
	void printEvents();
	void addToTotalTime(float newTime);
	void addToTotalDistance(float newDistance, float scaler);
	std::vector<Event> getEventsParticipated();
	
private:
	//first name
	std::string fName;
	//last name
	std::string lName;
	//country
	std::string country;
	//the total ammount of points they scored
	int totalPoints;
	//total amount of m they threw objects
	float totalDistance;
	//total 
	float totalTime;
	//vector of all of the events with a score of 0
	std::vector<Event> allEventVector;
	//vector of the events they got a score in
	std::vector<Event> eventsParticipated;

};
//return true if first goes above second
bool scoreCompare (const Athlete& a, const Athlete& b);
bool pointCompare (const Athlete& a, const Athlete& b);
bool customCompare (const Athlete& a, const Athlete& b);


#endif

