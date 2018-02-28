#ifndef _TEAM_H_
#define _TEAM_H_

#include <string>
#include <iostream>
#include <list>

#include "superhero.h"

class Team {

public:
  Team() : superheroes(std::list<Superhero>()) {}
  // accessor function
  std::string getName() const;
  // std::list<Superhero>::iterator begin(){
  //   return superheroes.begin();
  // }
  // std::list<Superhero>::iterator end(){
  //   return NULL;
  // }
  // adding and removing a single superhero
  void operator+=(const Superhero &a){
    superheroes.push_back(a);
  }
  void operator-=(const Superhero &a){
    std::list<Superhero>::iterator itr;
    for(itr= superheroes.begin(); itr!= superheroes.end(); ++itr){
      if(*itr==a){
        superheroes.erase(itr);
      }
    }
  }

  // adding two teams together or a team and a superhero together
  // makes a team.  these functions must be member functions to allow
  // access to the list of superheroes.
  friend Team operator+(const Team &a, const Team &b){
    std::list<Superhero>::const_iterator itrA;
    std::list<Superhero>::const_iterator itrB;
    Team newTeam;
    for(itrA= a.superheroes.begin(); itrA!=a.superheroes.end(); itrA++){
      newTeam+=*itrA;
    }
    for(itrB=b.superheroes.begin(); itrB!=b.superheroes.end(); itrB++){
      bool okToAdd= true;
      for(std::list<Superhero>::iterator ntItr = newTeam.superheroes.begin(); ntItr!=newTeam.superheroes.end(); ntItr++){
        if(*itrB==*ntItr){
          okToAdd=false;
          break;
        }
      }
      if(okToAdd){
        newTeam+=*itrB;
      }
    }
    return newTeam;
  }
  friend Team operator+(const Team &a, const Superhero &b){
    Team newTeam;
    newTeam+=b;
    newTeam= a+newTeam;
    return newTeam;

  }
  
private:
  // REPRESENATION
  std::list<Superhero> superheroes;
};

// non-member function because it doesn't need access to any Team variables
inline Team operator+(const Superhero &a, const Superhero &b){
  Team newTeam;
  newTeam+=a;
  newTeam+=b;
  return newTeam;
}

#endif
