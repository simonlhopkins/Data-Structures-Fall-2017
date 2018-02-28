// A simple "caller ID" program

#include <iostream>
#include <vector>
#include <string>
#include <map>
//using namespace std;

// add a number, name pair to the phonebook
void add(std::map<int, std::string> &phonebook, int number, std::string const& name) {
  phonebook[number]=name;
}
// given a phone number, determine who is calling
void identify(std::map<int, std::string> &phonebook, int number) {
  
  std::map<int, std::string>::const_iterator itr;
  itr = phonebook.find(number);
  if(itr != phonebook.end()){
    std::cout<<itr->second<<" is calling"<<std::endl;
  }
  else{
    std::cout<<"UNASSIGNED"<<std::endl;
  }
}


int main() {
  // create the phonebook; initially all numbers are unassigned
  //vector<string> phonebook(10000, "UNASSIGNED");
  std::map<int, std::string> phonebook;
  // add several names to the phonebook
  add(phonebook, 1111, "fred");
  add(phonebook, 2222, "sally");
  add(phonebook, 3333, "george");

  // test the phonebook
  identify(phonebook, 2222);
  identify(phonebook, 4444);
}
