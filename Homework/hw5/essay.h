#ifndef Essay_h_
#define Essay_h_

#include <iostream>
#include <cassert>
#include <string>


class Node{
public:
	Node(){
		next_=NULL;
		prev_=NULL;
		backup_next_=NULL;
		backup_prev_=NULL;
		added_ = false;
		removed_ = false;
	}
	Node(const std::string &v){
		next_=NULL;
		prev_=NULL;
		backup_next_=NULL;
		backup_prev_=NULL;
		value_=v;
		added_ = false;
		removed_ = false;
	}

	std::string value_;
	Node* prev_;
	Node* next_;
	Node* backup_next_;
	Node* backup_prev_;
	bool added_;
	bool removed_;

	~Node(){
		delete next_;
		next_=NULL;
		delete prev_;
		prev_=NULL;
		delete backup_next_;
		backup_next_=NULL;
		delete backup_prev_;
		backup_prev_=NULL;
		added_ = false;
		removed_ = false;
	}
};

class list_iterator{
public:
	list_iterator(Node *p= NULL){
		ptr = p;
	}
	std::string& operator*(){
		return ptr->value_;
	}
	Node* ptr;

};

class Essay{
public:
	Essay(){
		Node* head_= NULL;
		Node* tail_ = NULL;
		Node* backup_head_ = NULL;
		Node* backup_tail_ = NULL;
		size=0;
	}

	

	void push_back(const std::string &newString){
		std::cout<<"push back"<<std::endl;
		Node* newNode= new Node(newString);

		newNode->next_= NULL;
		newNode->prev_= tail_;
		//tail at the very beginning
		//std::cout<<"head is : "<<head_<<std::endl;
		if(head_){
			std::cout<<"head is null"<<std::endl;
			tail_ = newNode;
			head_ = newNode;
			head_ -> next_= NULL;
		}
		else{
			std::cout<<"head is not null"<<std::endl;
			tail_->next_= newNode;
		}
		tail_=newNode;

		
		//head_ = newNode;

		// std::cout<<"HEAD IS: "<<head_<<std::endl;
		// std::cout<<"TAIL IS: "<<tail_<<std::endl;
		// std::cout<<"ADDED NODE NEXT IS: "<<newNode->next_<<std::endl;
		//std::cout<<"ADDED NODE PREV IS: "<<newNode->prev_<<std::endl;
		
		
		size+=1;

	}

	void print(){
		// Node* temp= head_;
		// while(temp != NULL){
		// 	std::cout<<temp->value_<<" "<<std::endl;
		// 	temp =temp ->next_;
		// }
	}

	~Essay(){
		std::cout<<"Essay is getting destroyed"<<std::endl;
		while(head_ != NULL){
			Node* tempHead= head_;
			delete head_;
			head_=tempHead->next_;
		}
	}


private:
	Node* head_;
	Node* tail_;
	Node* backup_head_;
	Node* backup_tail_;
	int size;
};



#endif