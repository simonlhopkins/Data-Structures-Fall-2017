#include <vector>
#include <list>
#include <map>
#include "visualization.h"


#ifndef HASHTABLE_H
#define HASHTABLE_H


//helper functor used to sort the sets of <Point, int> pairs, sorting by the second int value,
//which is jus the raw hash value
class sortOrder{
public:
	bool operator()(const std::pair<Point, int> &a, const std::pair<Point, int> &b) {
       	return a.second>b.second;
    }
};

//This is my hash table class, which is an array. In each array slot there is a list of pairs. Those pairs
//are Point, int pairs, with the point being the point that was hashed to that index, and the int being
//the raw hash value. That raw hash value is useful for identifying if a point if equal to another point,
//since the index can be innacurate since it is the hash%tableSize, so you lose a lot of accuracy.
class HashTable{
public:
	HashTable(int tableSize_): tableSize(tableSize_) {
		table= new std::list<std::pair<Point, int> >[tableSize];
	}
	int hash(std::vector<int> values);

	std::pair<Point, int> find(const std::pair<Point, int> &point, int key);

	void insert(int key, Point value);

	std::list<std::pair<Point, int> >* getTable(){
		return table;
	}


private:
	int tableSize;
	std::list<std::pair<Point, int> >* table;
};

//hash function derived from the example shown in the lecture notes.
int HashTable::hash(std::vector<int> values){
	
	int hash = 1315423911;
	for(unsigned int i = 0; i < values.size(); i++){
		hash ^= ((hash << 13) + values[i] + (hash >> 2));

	}
	return std::abs(hash);

	
}


std::pair<Point, int> HashTable::find(const std::pair<Point, int> &point, int key){
	//loops through the index of the hash function that you pass in, and checks to see if
	//the second value is actually equal to the hash value, meaning it is an actual matcg
	for(std::list<std::pair<Point, int> >::iterator itr = table[key%tableSize].begin();
		itr!= table[key%tableSize].end(); itr++){
		if(itr->second == point.second){
			return *itr;
		}
		
	}

	std::cerr<<"point not found"<<std::endl;
	return std::pair<Point, int>(Point(0,0), -1);
}


void HashTable::insert(int key, Point value){
	//inserts at location key% tablesize, ensureing that it always has a position to insert at
	table[key%tableSize].push_back(std::pair<Point, int>(value, key));
}




#endif