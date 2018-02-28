#ifndef _SUPERHERO_H_
#define _SUPERHERO_H_

#include <string>
#include <map>
#include <vector>
#include <iostream>
class Superhero{
public:
	Superhero() : heroName(""), trueIdentity(""), power(""), isGood_(true) {}
	Superhero(std::string heroName_, std::string trueIdentity_, std::string power_) :
				heroName(heroName_), trueIdentity(trueIdentity_), power(power_), isGood_(true) {}
	std::string getName() const{ return heroName; }
	std::string getPower() const{ return power; }
	bool isGood(){return isGood_;}
	std::string getTrueIdentity() const{ return trueIdentity; }
	bool operator==(const Superhero &other) const{
    	return(this->trueIdentity== other.trueIdentity &&
    			this->power == other.getPower() &&
    			this->heroName == other.getName());
    }
    bool operator==(std::string secretGuess){
    	return(this->trueIdentity== secretGuess);
    }
    bool operator!=(std::string secretGuess){
    	return(this->trueIdentity!= secretGuess);
    }

    void operator-(){
    	isGood_ =!isGood_;
    }

    bool operator>(const Superhero &other){
    	std::vector<std::string> powers;
    	powers.push_back("Flexible");
    	powers.push_back("Flying");
    	powers.push_back("Invisible");
    	powers.push_back("Laser");
    	powers.push_back("Fire");
    	powers.push_back("Wood");
    	powers.push_back("Water");
    	powers.push_back("Speed");
    	std::map<std::string, std::vector<std::string> > powerRankings=makePowerMap(powers);
    	powerRankings["Wood"].push_back("Water");
    	powerRankings["Fire"].push_back("Wood");
    	powerRankings["Water"].push_back("Fire");
    	powerRankings["Flying"].push_back("Fire");
		powerRankings["Flying"].push_back("Water");
		powerRankings["Flying"].push_back("Wood");

    	for(int i=0; i< powerRankings[this->power].size(); i++){
    		if(powerRankings[this->power][i]==other.getPower()){
    			return true;
    		}
    	}
    	return false;
    }
    std::map<std::string, std::vector<std::string> > makePowerMap(std::vector<std::string> allPowers){
    	std::map<std::string, std::vector<std::string> > newMap;
    	for(int i=0; i< allPowers.size();i++){
    		
    		std::vector<std::string> newVec;
    		newMap.insert(std::pair<std::string, std::vector<std::string> >(allPowers[i], newVec));
    		

    	}
    	return newMap;
    }

private:
	std::string heroName;
	std::string trueIdentity;
	std::string power;
	bool isGood_;



};

inline std::ostream& operator<<(std::ostream& os, Superhero hero){  
    os << hero.getName()<<std::endl;
    return os; 
}


#endif