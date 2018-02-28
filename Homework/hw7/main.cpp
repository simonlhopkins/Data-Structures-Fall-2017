#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

typedef std::map<std::string, std::pair<int, std::vector<std::pair<std::string, std::string> > > > COSTUME_SHOP_TYPE;
typedef std::map<std::pair<std::string, std::string>, std::string> PEOPLE_TYPE;

// prototypes for the helper functions
void addCostume(COSTUME_SHOP_TYPE &costume_shop);
void rentCostume(COSTUME_SHOP_TYPE& costume_shop, PEOPLE_TYPE& people);
void lookup(COSTUME_SHOP_TYPE& costume_shop);
void printPeople(PEOPLE_TYPE& people);
//bool usingFile=false;
int main() {
  // two data structures store all of the information for efficiency
  COSTUME_SHOP_TYPE costume_shop;
  PEOPLE_TYPE people;
  

  char c;
  
  while (std::cin >> c) {
    if (c == 'a') {
      addCostume(costume_shop);
    } else if (c == 'r') {
      rentCostume(costume_shop,people);
    } else if (c == 'l') {
      lookup(costume_shop);
    } else if (c == 'p') {
      printPeople(people);
    } else {
      std::cerr << "error unknown char " << c << std::endl;
      exit(0);
    }
  }
}


void addCostume(COSTUME_SHOP_TYPE& costume_shop) {
  std::string costume_name;
  int num;
  std::cin >> costume_name;
  std::cin >> num;
  COSTUME_SHOP_TYPE::iterator itr;
  itr= costume_shop.find(costume_name);
  if(itr!= costume_shop.end()){
    (itr->second).first +=num;
  }
  else{
    costume_shop[costume_name]= std::pair<int, std::vector<std::pair<std::string, std::string> > >(num, std::vector<std::pair<std::string, std::string> >());
    
  }
  if(num==1){
    std::cout<<"Added "<<num<<" "<<costume_name<<" costume."<<std::endl;
  }else{
    std::cout<<"Added "<<num<<" "<<costume_name<<" costumes."<<std::endl;

  }

}


void rentCostume(COSTUME_SHOP_TYPE& costume_shop, PEOPLE_TYPE& people) {
  std::string first_name;  
  std::string last_name;
  std::string costume_name;
  std::cin >> first_name >> last_name >> costume_name;
  
  COSTUME_SHOP_TYPE::iterator csItr;
  csItr= costume_shop.find(costume_name);
  PEOPLE_TYPE::iterator pItr;
  pItr= people.find(std::pair<std::string, std::string>(last_name, first_name));

  
  //the person does not exsist yet
  if(pItr==people.end()){
    //costume does not exsist
    if(csItr==costume_shop.end()){
      std::cout<<"The shop doesn't carry "<<costume_name<<" costumes."<<std::endl;
      people[std::pair<std::string, std::string>(last_name, first_name)]= "";
      return;
    }
    if(costume_shop[costume_name].second.size()==costume_shop[costume_name].first){
      std::cout<<"No "<<costume_name<<" costumes available."<<std::endl;
      people[std::pair<std::string, std::string>(last_name, first_name)]= "";
      return;
    }

    people[std::pair<std::string, std::string>(last_name, first_name)]= costume_name;
    costume_shop[costume_name].second.push_back(std::pair<std::string, std::string>(last_name, first_name));
    std::cout<<first_name+" "+last_name<<" rents a "<<costume_name<<" costume."<<std::endl;

    
  }
  //person already exsists in the list
  else{
    //if the costume does not exsist
    if(csItr==costume_shop.end()){
      std::cout<<"The shop doesn't carry "<<costume_name<<" costumes."<<std::endl;
      //people[std::pair<std::string, std::string>(last_name, first_name)]= "";
      return;
    }

    //if the person already has a costume they are trying to rent
    if(people[std::pair<std::string, std::string>(last_name, first_name)]==costume_name){
      std::cout<<first_name+" "+last_name<<" already has a "<<costume_name<<" costume."<<std::endl;
      return;
    }
    //there are as many costumes rented out that exist in the shop
    if(costume_shop[costume_name].second.size()==costume_shop[costume_name].first){
      std::cout<<"No "<<costume_name<<" costumes available."<<std::endl;
      return;
    }

    //loops through all the people in the costumes renter list
    for(int i=0; i<costume_shop[people[std::pair<std::string, std::string>(last_name, first_name)]].second.size();i++){
      //if the person is in the list
      if(costume_shop[people[std::pair<std::string, std::string>(last_name, first_name)]].second[i]==std::pair<std::string, std::string>(last_name, first_name)){
        //erase that person from the list
        costume_shop[people[std::pair<std::string, std::string>(last_name, first_name)]].second.erase(costume_shop[people[std::pair<std::string, std::string>(last_name, first_name)]].second.begin()+i);
      }
    }
    //adds them to their newly rented out costume list
    costume_shop[costume_name].second.push_back(std::pair<std::string, std::string>(last_name, first_name));
    if(people[std::pair<std::string, std::string>(last_name, first_name)]==""){
      std::cout<<first_name+" "+last_name<<" rents a "<<costume_name<<" costume."<<std::endl;
    }
    else{
      std::cout<<first_name+" "+last_name+" returns a "<<people[std::pair<std::string, std::string>(last_name, first_name)] 
      <<" costume before renting a "<<costume_name<<" costume."<<std::endl;
    }


    

    people[std::pair<std::string, std::string>(last_name, first_name)]= costume_name;

  }
  //costume exsists in the shop
  
  



}


void lookup(COSTUME_SHOP_TYPE& costume_shop) {
  std::string costume_name;
  std::cin >> costume_name;
  COSTUME_SHOP_TYPE::iterator csItr;
  csItr= costume_shop.find(costume_name);
  if(csItr==costume_shop.end()){
    std::cout<<"The shop doesn't carry "<<costume_name<<" costumes."<<std::endl;
    return;
  }
  std::cout<<"Shop info for "<<costume_name<<" costumes:"<<std::endl;
  int copiesAv= costume_shop[costume_name].first-costume_shop[costume_name].second.size();
  int copiesRented= costume_shop[costume_name].second.size();
  if(copiesAv>0){
    if(copiesAv==1){
      std::cout<<"  "<<copiesAv<<" copy available"<<std::endl;
    }
    else{
      std::cout<<"  "<<copiesAv<<" copies available"<<std::endl;
    }
  }
  if(copiesRented>0){
    if(copiesRented==1){
      std::cout<<"  "<<copiesRented<<" copy rented by:"<<std::endl;
    }else{
      std::cout<<"  "<<copiesRented<<" copies rented by:"<<std::endl;
    }
    for(int i=0; i<copiesRented;i++){
      std::cout<<"    "<<costume_shop[costume_name].second[i].second<<" "<<costume_shop[costume_name].second[i].first<<std::endl;
    }
  }



}


void printPeople(PEOPLE_TYPE& people) {
  if(people.size()==1){
    std::cout<<"Costume info for the "<<people.size()<<" party attendee:"<<std::endl;
  }else{
    std::cout<<"Costume info for the "<<people.size()<<" party attendees:"<<std::endl;
  }
  PEOPLE_TYPE::iterator pItr;
  pItr= people.begin();
  while(pItr!= people.end()){
    if(pItr->second==""){
      std::cout<<"  "<<(pItr->first).second<<" "<< (pItr->first).first<<" does not have a costume."<<std::endl;
    } else{
      std::cout<<"  "<<(pItr->first).second<<" "<< (pItr->first).first<<" is wearing a "<<pItr->second<<" costume."<<std::endl;
    }
    pItr++;
    
  }




}
