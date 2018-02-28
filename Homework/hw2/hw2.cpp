//all neccisary includes
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include "event.h"
#include "athlete.h"

//creates a vector containing all of the athletes and a vector of all possible events available
std::vector<Athlete> athleteVector;
std::vector<Event> eventArray;

//function that returns true if targetAthlete is already in athlete vector
bool athleteAlreadyExists(Athlete targetAthlete){
	for(int i=0; i<athleteVector.size(); i++){
		if(athleteVector[i].getFullName()==targetAthlete.getFullName()){
			return true;
		}
	}
	return false;

}

//this returns a copy of an event object from the event vector by the name of the event
Event getEventByName(std::string eventName){
	for(int i=0; i<eventArray.size();i++){
		if(eventArray[i].getName()==eventName){
			return eventArray[i];
		}
	}
	std::cout<<"EVENT DOESN'T EXSIST"<<std::endl;
	return eventArray[0];
}

//this returns a reference to an Athlete by their name by searching through the athlete vector
Athlete &getAthleteByName(std::string athleteName){
	for(int i=0; i<athleteVector.size();i++){
		if(athleteVector[i].getFullName()==athleteName){
			return athleteVector[i];
		}
	}
	std::cout<<"ATHLETE DOESN'T EXSIST"<<std::endl;
	return athleteVector[0];
}



int main(int argc, char *argv[]){

	//defines all of the different types of events in the decathalon, 
	eventArray.push_back(Event("100_METERS", 25.4347, 18, 1.81, true, 1, "100"));
	eventArray.push_back(Event("LONG_JUMP", 0.14354, 220, 1.4, false, 100, "LJ"));
	eventArray.push_back(Event("SHOT_PUT", 51.39, 1.5, 1.05, false, 1, "SP"));
	eventArray.push_back(Event("HIGH_JUMP", 0.8465, 75, 1.42, false, 100, "HJ"));
	eventArray.push_back(Event("400_METERS", 1.53775, 82, 1.81, true, 1, "400"));
	eventArray.push_back(Event("110_METERS_HURDLES", 5.74352, 28.5, 1.92, true, 1, "110H"));
	eventArray.push_back(Event("DISCUS_THROW", 12.91, 4, 1.1, false, 1, "DT"));
	eventArray.push_back(Event("POLE_VAULT", 0.2797, 100, 1.35, false, 100, "PV"));
	eventArray.push_back(Event("JAVELIN_THROW", 10.14, 7, 1.08, false, 1, "JT"));
	eventArray.push_back(Event("1500_METERS", 0.03768, 480, 1.85, true, 1, "1500"));
	//creates an ifstream object and opens the first argument as a file name
	std::ifstream dataFile;
	dataFile.open(argv[1]);
	//the current word the ifstream is on
	std::string word;
	//this variable is the current event section that the ifstream is in, and changes when it
	//reaches another event keyword
	std::string currentEvent;

	
	//READS THE FILE
	dataFile >> word;
	while(dataFile.good()){

		//checks if the current word is event
		if(word.compare("event")==0){
			//goes to next line if it is
			dataFile >> word;
			//sets the current event to that word
			currentEvent=word;
			dataFile >> word;
		}
		//sets the first name, last name, and country to temp variables that will then
		//be entered into an Athlete object as parameters
		std::string firstName=word;
		dataFile >> word;

		std::string lastName=word;
		dataFile >> word;
		std::string country=word;
		Athlete newAthlete=Athlete(firstName,lastName, country, eventArray);
		//checks if newAthlete is already in the array, if it is it adds it,
		//otherwise it does nothing with it
		if(!athleteAlreadyExists(newAthlete)){
			athleteVector.push_back(newAthlete);
		}
		//creates a new event from a copy of the decathelon events I made earlier
		Event newEvent = getEventByName(currentEvent);

		dataFile >> word;
		std::string score= (word);
		//sets the score of the event that it just added
		newEvent.setScore(score);
		
		//adds new event to the athlete in the athlete vector
		getAthleteByName(firstName+" "+lastName).addEvent(newEvent);
		
		//this bit of code adds the event score to a total time or total distance variable in
		//athlete that will be used for my custom output.
		if(newEvent.getIsTrack()){
			//converts 1500 meters to a normalized value that is in seconds
			float scoreFloat;

			if(newEvent.getName()=="1500_METERS"){
				float min= std::stof(newEvent.getScore().substr(0, newEvent.getScore().find(":")));
				float sec= std::stof(newEvent.getScore().substr(newEvent.getScore().find(":")+1));

				scoreFloat=((60*min)+(sec));
			}
			else{
				scoreFloat=std::stof(newEvent.getScore());
			}

			//adds to the athlete's total time if it is a track event
			getAthleteByName(firstName+" "+lastName).addToTotalTime(scoreFloat);
		}else{
			//adds to the athlete's total distance (with constant scale) if it is a field event

			getAthleteByName(firstName+" "+lastName).addToTotalDistance(std::stof(newEvent.getScore()), newEvent.getScale());
		}
		

		dataFile >> word;
	}
		

	//WRITE TO A NEW FILE

	//creates an output stream I can write to
	std::ofstream fileToWriteTo;
	//sets the type of data it wants to read
	std::string dataType=argv[3];

	std::string fileName= argv[2];

	//opens up the file I want to write to
	fileToWriteTo.open (fileName);
	//sets the amount of spaces I want a word to take up depending on what type of data it is
	int nameWidth=15;
	int numWidth=10;
	int customWidth=25;
	char fillChar= ' ';
	std::string dataTypeCAPS= dataType;
	std::transform(dataTypeCAPS.begin(), dataTypeCAPS.end(), dataTypeCAPS.begin(), ::toupper);
	//for all cases it starts by writing DECATHLETE SCORES at the top
	fileToWriteTo<<std::left<<std::setfill(fillChar)<<std::setw((nameWidth*2)+numWidth)<<"DECATHLETE "+ dataTypeCAPS;
	//if the type is not custom it writes all of the event abbreviations on the top of the file
	//if it is it writes the custom headers
	if(dataType!="custom"){
		for(int i=0; i<eventArray.size();i++){
			fileToWriteTo<<std::left<<std::setfill(fillChar)<<std::setw(numWidth)<<eventArray[i].getAbbreviation();
		}
	}
	else{
		fileToWriteTo<<std::left<<std::setfill(fillChar)<<std::setw(customWidth)<<"TIME_SPENT_RUNNING (s)";
		fileToWriteTo<<std::left<<std::setfill(fillChar)<<std::setw(customWidth)<<"DISTANCE_OF_OBJECTS_THROWN (m)";

	}
	
	//if it is scores it adds an additional column TOTAL
	if(dataType=="points"){
		fileToWriteTo<<std::left<<std::setfill(fillChar)<<std::setw(numWidth)<<"TOTAL";
	}
	fileToWriteTo<<std::endl;


	//sorts the athletes depending on what the dataType is
	if(dataType=="scores"){
		std::sort(athleteVector.begin(), athleteVector.end(), scoreCompare);
	}
	else if(dataType=="points"){
		std::sort(athleteVector.begin(), athleteVector.end(), pointCompare);
	}
	else if(dataType=="custom"){
		std::sort(athleteVector.begin(), athleteVector.end(), customCompare);
	}

	for(int i=0; i<athleteVector.size();i++){

		//writes the athlete name and country for all cases
		fileToWriteTo<<std::left<<std::setfill(fillChar)<<std::setw(nameWidth)<<athleteVector[i].getFirstName();
		fileToWriteTo<<std::left<<std::setfill(fillChar)<<std::setw(nameWidth)<<athleteVector[i].getLastName();
		fileToWriteTo<<std::left<<std::setfill(fillChar)<<std::setw(numWidth)<<athleteVector[i].getCountry();
		//if not the custom case, it loops through each event for each athlete, and outputs either the score or the points
		if(dataType!="custom"){
			for(int j=0; j< athleteVector[i].getEventsParticipated().size();j++){
			// fileToWriteTo<<athleteVector[i].getEventsParticipated()[j].getName()+" "<<athleteVector[i].getEventsParticipated()[j].calcPoints()<<std::endl;
				if(athleteVector[i].getEventsParticipated()[j].getScore()=="0"){
					//outputs different characters for a 0 score depending on points or scores
					if(dataType=="scores"){
						fileToWriteTo<<std::left<<std::setfill(' ')<<std::setw(numWidth)<<" ";
					}else{
						fileToWriteTo<<std::left<<std::setfill(' ')<<std::setw(numWidth)<<"0";
					}

				}else{
					if(dataType=="scores"){
						fileToWriteTo<<std::left<<std::setfill(' ')<<std::setw(numWidth)<<athleteVector[i].getEventsParticipated()[j].getScore();
					}else if(dataType=="points"){
						fileToWriteTo<<std::left<<std::setfill(' ')<<std::setw(numWidth)<<athleteVector[i].getEventsParticipated()[j].calcPoints();
					}
					else{
						fileToWriteTo<<std::left<<std::setfill(' ')<<std::setw(numWidth)<<athleteVector[i].getEventsParticipated()[j].getTotalPoints();
					}

				}
				

			}
		}
		//writes the custom calculation
		else{
			fileToWriteTo<<std::left<<std::setfill(fillChar)<<std::setw(customWidth)<<athleteVector[i].getTotalTime();
			fileToWriteTo<<std::left<<std::setfill(fillChar)<<std::setw(customWidth)<<athleteVector[i].getTotalDistance();

		}

		//writes the total points on the end if it is dataType points
		if(dataType=="points"){
			fileToWriteTo<<std::left<<std::setfill(' ')<<std::setw(numWidth)<<athleteVector[i].getTotalPoints();
		}
		fileToWriteTo<<std::endl;
	}

	fileToWriteTo.close();


	return 0;
}