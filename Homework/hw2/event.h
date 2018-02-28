#include <vector>
#include <string>
#include <iostream>
#ifndef EVENT_H
#define EVENT_H

//this is a class that holds all of the information for an event, with an initial score of 0

class Event{
public:
	Event(const std::string eName, const float eA, const float eB, const float eC, const bool eIsTrack,
				 float scale, std::string eAbbreviation);

	//GETTERS
	std::string getName() const;
	bool getIsTrack() const;
	int calcPoints();
	std::string getScore() const;
	std::string getAbbreviation() const;
	float getScale() const;
	float getTotalPoints() const;

	//SETTERS
	void setScore(std::string newScore);
	void setPoints(int newPoints);

	float eventTotal;
	void addToEventTotal(float numToAdd);


private:
	//name of event
	std::string name;
	//A,B,and C coefficients
	float A;
	float B;
	float C;
	//the score as a string
	std::string score;
	//the factor of a meter each event is
	float scale;
	//true if track event, false if field event
	bool isTrack;
	//point value
	int points;
	//abreviation
	std::string abbreviation;
};

//used for comparing if 2 events are equal to each other
bool operator== (const Event& a, const Event& b);


#endif