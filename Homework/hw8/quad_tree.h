// ===================================================================
//
// We provide the Point class and the implementation of several
// QuadTree member functions for printing.  
//
// IMPORTANT: You should modify this file to add all of the necessary
// functionality for the QuadTree class and its helper classes: Node,
// depthIterator, and breadthIterator.
//
// ===================================================================

#ifndef quad_tree_h_
#define quad_tree_h_

#include <iostream>
#include <vector>
#include <cassert>
#include <list>


// ==============================================================
// ==============================================================
// A tiny templated class to store 2D coordinates.  This class works
// with number_type = int, float, double, unsigned char (a 1 byte=8
// bit integer), short (a 2 byte=16 bit integer).
template <class number_type, class label_type> class QuadTree;

template <class number_type>
class Point {
public:
  Point(const int nullVal) : x(NULL), y(NULL) {}
  Point(const number_type& x_, const number_type& y_) : x(x_),y(y_) {}
  // REPRESENTATION
  number_type x;
  number_type y;
};

template <class number_type>
bool operator== (Point<number_type> p1, Point<number_type> p2){
  if(p1.x==p2.x && p1.y==p2.y){
    return true;
  }
  return false;
}

// a helper function to print Points to STL output stream
template <class number_type>
inline std::ostream& operator<<(std::ostream &ostr, const Point<number_type> &pt) {
  ostr << "(" << pt.x << "," << pt.y << ")";
  return ostr;
}


//Node is a data type that holds a point and a label. The nodes are what are plotted on the 2D graph
template <class number_type, class label_type>
class Node{
public:
  Node(): pt(NULL), label(label_type()){}
  //sets all the children to NULL by default
  Node(const Point<number_type> pt_, label_type label_) : pt(pt_), label(label_){
    for(int i=0; i<4;i++){
      children[i]=NULL;
    }
    parent=NULL;
  }
  Point<number_type> pt;
  label_type label;
  //array of pointers
  Node<number_type,label_type>* children[4];
  Node<number_type,label_type>* parent;


};

//the depth iterator traverses the tree depth first, going down the list starting at the root from the 0th child to the 4th child
template <class number_type, class label_type>
class depthIterator{
public:

  //constructors
  depthIterator() : ptr_(NULL), tree_(NULL) {}
  depthIterator(Node<number_type, label_type>* p, const QuadTree<number_type, label_type>* q) : ptr_(p), tree_(q){}
  depthIterator& operator=(const depthIterator& old) { ptr_ = old.ptr_;  tree_ = old.tree_; return *this; }
  //operators
  depthIterator(const depthIterator& old) : ptr_(old.ptr_), tree_(old.tree_) {}
  friend bool operator== (const depthIterator& lft, const depthIterator& rgt)
  { return (lft.tree_ == rgt.tree_ && lft.ptr_ == rgt.ptr_); }
  friend bool operator!= (const depthIterator& lft, const depthIterator& rgt)
  { return (lft.tree_ != rgt.tree_ || lft.ptr_ != rgt.ptr_); }
  const Point<number_type>& operator*() const { return ptr_->pt; }

  //destructor
  ~depthIterator() {}
  //getters
  label_type getLabel(){ return ptr_->label; }
  int const getDepth();
  //helper function to incriment the iterator
  void incriment(Node<number_type, label_type>* n);
  //incriment operator for depth first
  depthIterator<number_type, label_type> & operator++() {
    //if the node you are incrimenting is NULL return NULL
    if(ptr_==NULL){
      ptr_=NULL;
      return *this;
    }
    //first check if you can go to any of the child nodes of root
    for(int i=0; i<4; i++){
      if(ptr_->children[i]!=NULL){
        //if one of the children exsists then return that child return an iterator at that child
        ptr_=ptr_->children[i];
        return *this;
      }
    }
    //if not incriment
    incriment(ptr_);
    return *this;
  }
  //post inciment operator
  depthIterator<number_type, label_type> operator++(int) {
    depthIterator<number_type, label_type> temp(*this);
    ++(*this);
    return temp;
  }

private:
  // representation
  Node<number_type, label_type>* ptr_;
  const QuadTree<number_type, label_type>* tree_;
};

//this function recuresively goes down the tree by first finding where the current node lies
//relative to its parents children, and then checking if there are any children ahead of it.
//if that is not the case it goes one right of the parent. If it can't do that it returns NULL
//because it must be the last one
template <class number_type, class label_type>
void depthIterator<number_type, label_type>::incriment(Node<number_type, label_type>* n){
  //look for any siblings in front of the curent pointer n
  if(n->parent==NULL){
    ptr_=NULL;
    return;
  }
  int startingIndex=-1;
  for(int i=0; i<4; i++){
    if(n->parent->children[i]==n){
      startingIndex=i;
    }
  }
  //it is not the last one i.e. 0, 1, 2
  if(startingIndex<3){
    for(int i=startingIndex+1; i<4; i++){
      if(n->parent->children[i]!=NULL){
        ptr_=n->parent->children[i];
        return;
      }
    }
  }
  incriment(n->parent);
  
}


//gets depth by going up the tree until it reaches the root node.
template <class number_type, class label_type>
int const depthIterator<number_type, label_type>::getDepth(){
  int depthSoFar=0;
  Node<number_type, label_type>* currentNode=ptr_;
  while(currentNode->parent!=NULL){
    depthSoFar+=1;
    currentNode=currentNode->parent;
  }
  return depthSoFar;
  
}

//this iterator goes through the list level by level, aka breadth first
template <class number_type, class label_type>
class breadthIterator{
public:
  //constructors
  breadthIterator() : ptr_(NULL), tree_(NULL){}
  breadthIterator(Node<number_type, label_type>* n, const QuadTree<number_type, label_type>* q) : ptr_(n), tree_(q){
    //if the root node exsists
    if(tree_->root_!=NULL){
      //make the initial current level the root node
      currentLevel.push_back(tree_->root_);
      //sets the initial position of the current level iterator
      cuItr= currentLevel.begin();
      //sets the root's children to the initial next level list
      for(int i=0; i<4; i++){
        if((*cuItr)->children[i]!=NULL){
          nextLevel.push_back((*cuItr)->children[i]);
        }
      }
    }
    else{
      ptr_=NULL;
    }
  }
  //operators
  friend bool operator== (const breadthIterator& lft, const breadthIterator& rgt)
  { return (lft.tree_ == rgt.tree_ && lft.ptr_ == rgt.ptr_); }
  friend bool operator!= (const breadthIterator& lft, const breadthIterator& rgt)
  { return (lft.tree_ != rgt.tree_ || lft.ptr_ != rgt.ptr_); }
  breadthIterator& operator=(const breadthIterator& old) { ptr_ = old.ptr_;  tree_ = old.tree_; return *this; }
  
  ~breadthIterator() {}
  label_type getLabel(){return ptr_->label;}
  const Point<number_type>& operator*() const { return ptr_->pt; }
  int const getDepth();
  breadthIterator<number_type, label_type> & operator++() { 
    //incriment the current level iterator
    ++cuItr;
    //if I reach the end of the current list and find nothing
    if(cuItr==currentLevel.end()){
      //if the next level has no values then return a null pointer, you've reached the end
      if(nextLevel.empty()){
        ptr_=NULL;
        return *this;
      }
      //sets the current level to the nect level
      currentLevel = nextLevel;
      //sets the current level iterator to the first value of the new current level
      cuItr = currentLevel.begin();
      //clears the next level
      nextLevel.clear();
      //adds the first value's children to the next level, which is now clear
      for(int i=0; i<4;i++){
        //if it is not a null value in children
        if((*cuItr)->children[i]!=NULL){
          //add the child
          nextLevel.push_back((*cuItr)->children[i]);
        }
      }
    }
    //if the next value is not null
    else{
      //add all of the next values children to the next level
      for(int i=0; i<4; i++){
        if((*cuItr)->children[i]!=NULL){
          nextLevel.push_back((*cuItr)->children[i]);
        }
      }
    }
    //make pointer equal to the iterator value;
    ptr_ = *cuItr;
    return *this;

  }
  breadthIterator<number_type, label_type> operator++(int) {
    breadthIterator<number_type, label_type> temp(*this);
    ++(*this);
    return temp;
  }

private:
  std::list<Node<number_type, label_type>* > currentLevel;
  std::list<Node<number_type, label_type>* > nextLevel;
  Node<number_type, label_type>* ptr_;
  typename std::list<Node<number_type, label_type>* >::iterator cuItr;
  const QuadTree<number_type, label_type>* tree_;
};


template <class number_type, class label_type>
//gets the depth by going up from the node until it reaches the parent node
int const breadthIterator<number_type, label_type>::getDepth(){
  int depthSoFar=0;
  Node<number_type, label_type>* currentNode=ptr_;
  while(currentNode->parent!=NULL){
    depthSoFar+=1;
    currentNode=currentNode->parent;
  }
  return depthSoFar;
  
}

//this class is the manager for all of the nodes. It houses the root node pointer, as well as a bunch
//of other helpful functions to get information about the tree, as well as add information to the tree.
template <class number_type, class label_type>
class QuadTree{
public:
  //constructors
  QuadTree();
  QuadTree(const QuadTree<number_type, label_type>& old) : size_(old.size_), height_(old.height_) { root_=this->copy_tree(old.root_,NULL); }
  QuadTree & operator=(const QuadTree<number_type, label_type>& old) {
    if (&old != this) {
      this->deleteTree(root_);
      root_ = this->copy_tree(old.root_,NULL);
      size_ = old.size_;
      height_= old.height_;
    }
    return *this;
  }
  
  //iterators------------------------------------------------------
  friend class depthIterator<number_type, label_type>;
  typedef depthIterator<number_type, label_type> iterator;
  friend class breadthIterator<number_type, label_type>;
  typedef breadthIterator<number_type, label_type> bf_iterator;
  //---------------------------------------------------------------
  int const height() const {return height_;}
  int const size() const {return size_;}
  void plot(Node<number_type,label_type> *p, std::vector<std::string> &board, 
            int x_min, int x_max, int y_min, int y_max, bool draw_lines) const;
  void plot(int max_x, int max_y, bool draw_lines=true) const;

  void print_sideways() const;
  void print_sideways(Node<number_type,label_type>* p, const std::string &indent) const;
  QuadTree<number_type, label_type>::iterator find(number_type x, number_type y);
  std::pair< iterator, bool > insert(const Point<number_type> &p, const label_type &label);
  //the begin iterator is set to an iterator at the root
  iterator begin() const { 
    if (!root_) return iterator(NULL,this);
    return(iterator(root_, this));
  }
  //the end is set to an iterator of NULL
  iterator end() const { return iterator(NULL,this); }
  //the bf_begin iterator is set to an bf_iterator at the root
  bf_iterator bf_begin() const{
    if (!root_) return bf_iterator(NULL,this);
    return(bf_iterator(root_, this));
  }
  //the end is set to an bf_iterator of NULL

  bf_iterator bf_end() const {return bf_iterator(NULL, this); }
  //helper recursive function to delete the tree from the leaf up
  void deleteTree(Node<number_type, label_type>* currentNode);
  //destructor which calls the delete tree function recursively
  ~QuadTree(){
    deleteTree(root_);
    root_=NULL;
  }
  
private:
  int size_;
  int height_;
  Node<number_type,label_type>* root_;
  //copy tree helper function for assignment and copy constructor
  Node<number_type,label_type>* copy_tree(Node<number_type,label_type>* old_root, Node<number_type,label_type>* the_parent) {
   if (old_root == NULL) return NULL;
   Node<number_type,label_type>* answer = new Node<number_type,label_type>();
   //sets the new base root's values to the old root's values
   answer->pt= old_root->pt;
   answer->label = old_root->label;
   answer->children[0] = copy_tree(old_root->children[0],answer);
   answer->children[1] = copy_tree(old_root->children[1],answer);
   answer->children[2] = copy_tree(old_root->children[2],answer);
   answer->children[3] = copy_tree(old_root->children[3],answer);
   answer->parent = the_parent;
   return answer;
   }


};

//delete tree helper function
template <class number_type, class label_type>
void QuadTree<number_type, label_type>::deleteTree(Node<number_type, label_type>* currentNode){
  //loops through all of current node's children
  for(int i=0; i<4; i++){
    //if one of the children exsists
    if(currentNode->children[i]!=NULL){
      //call delete tree on that node
      deleteTree(currentNode->children[i]);
    }
  }


  //all of the node's children are null
  
  currentNode->parent=NULL;
  //sets the parents child node for that spot to null;

  for(int i=0; i<4; i++){
    assert(currentNode!=NULL);
    //sets all of it's children
    currentNode->children[i]=NULL;
  }
  //deletes it if it has no children
  delete currentNode;



}
//default empty constructor for quadtree
template <class number_type, class label_type>
QuadTree<number_type, label_type>::QuadTree(){
  size_= 0;
  height_=-1;
  root_=NULL;

}
//find returns a pair of type iterator of either the value or a NULL iterator if it didn't find it
template <class number_type, class label_type>
typename QuadTree<number_type, label_type>::iterator QuadTree<number_type, label_type>::find(number_type x, number_type y){
  Node<number_type, label_type>* currentNode= root_;
  //tracker keeps track of what child index it found the item in
  int tracker=-1;
  //while the point calues don't equal the values you passed into the function
  while(currentNode->pt.x!=x && currentNode->pt.y!=y){
    //figures out what child it should go to
    if(y<currentNode->pt.y){
      if(x<currentNode->pt.x){
        tracker=0;
      }
      else{
        tracker=1;
      }
    }
    else{
      if(x<currentNode->pt.x){
        tracker=2;
      }
      else{
        tracker=3;
      }
    }
    //if it reaches the end and points to nothing, return NULL
    if(currentNode->children[tracker]==NULL){
      return iterator(NULL,this);
    }
    else{
      //if the child it goes to does not equal NULL, then go to that child and loop ahaains
      currentNode= currentNode->children[tracker];
    }
  }
  //return an iterator at current node if it finds something.
  return iterator(currentNode,this);
  
}
//insert function returns an iterator of the new node that you just inserted, and a bool value that states if 
//the value was actually inserted
template <class number_type, class label_type>
std::pair< typename QuadTree<number_type, label_type>::iterator, bool > QuadTree<number_type, label_type>::insert(const Point<number_type> &p, const label_type &label){
  //if there is nothing in the tree
  //if the root is null then insert the value at the root
  if(root_==NULL){
    Node<number_type, label_type>* newNode= new Node<number_type, label_type>(p, label);
    root_=newNode;
    height_=0;
    size_+=1;
    return std::pair< iterator, bool >(iterator(newNode,this), true);
  }
  //keeps track of height
  int heightSoFar=1;
  Node<number_type, label_type>* currentNode = root_;
  //std::cout<<"the root before is: "<< root_->label<<std::endl;
  int tracker=-1;

  while(currentNode!=NULL){
    //if the current node equals the point you're inserting at, then return an iterator of current node
    //and set the second value to false
    if(p==currentNode->pt){
      return std::pair< iterator, bool >(iterator(currentNode,this), false);
    }
    if(p.y<currentNode->pt.y){
      //both x and y are less than
      if(p.x<currentNode->pt.x){
        tracker=0;
      }
      else{
        tracker=1;
      }
    }
    //checks if it should move on to the right two values
    else{
      if(p.x<currentNode->pt.x){
        tracker=2;
      }
      else{
        tracker=3;
      }
    }
    //if current node is not null, i.e. the value still exsists, then move down one level to the
    //tracker ith node and also make the height taller
    if(currentNode->children[tracker]!=NULL){
      currentNode=currentNode->children[tracker];
      heightSoFar+=1;
    }
    //else break out of this loop
    else{
      break;
    }
  }
  //make a new node
  Node<number_type, label_type>* newNode= new Node<number_type, label_type>(p, label);
  //set the parent to the node which's child was NULL
  newNode->parent= currentNode;
  //set the child of that node to the new node you just made
  currentNode->children[tracker]=newNode;
  //increase size
  size_+=1;
  //if the height was taller than before, reset the height
  if(heightSoFar>height_){
    height_=heightSoFar;
  }
  assert(root_!=NULL);
  //return that iterator
  return std::pair< iterator, bool >(iterator(newNode,this), true);

}


// ==============================================================
// PROVIDED CODE : QUAD TREE MEMBER FUNCTIONS FOR PRINTING
// ==============================================================

// NOTE: this function only works for quad trees with non negative
// integer coordinates and char labels

// NOTE2: this function assumes that no two points have the same x
// coordinate or the same y coordinate.

// plot driver function
// takes in the maximum x and y coordinates for these data points
// the optional argument draw_lines defaults to true
template <class number_type, class label_type>
void QuadTree<number_type, label_type>::plot(int max_x, int max_y, bool draw_lines) const {
  // allocate blank space for the center of the board
  std::vector<std::string> board(max_y+1,std::string(max_x+1,' '));
  // edit the board to add the point labels and draw vertical and
  // horizontal subdivisions
  plot(root_,board,0,max_x,0,max_y,draw_lines);
  // print the top border of the plot
  std::cout << "+" << std::string(max_x+1,'-') << "+" << std::endl;
  for (int i = 0; i <= max_y; i++) {
    // print each row of the board between vertical border bars
    std::cout << "|" << board[i] << "|" << std::endl;
  }
  // print the top border of the plot
  std::cout << "+" << std::string(max_x+1,'-') << "+" << std::endl;
}

// actual recursive function for plotting
template <class number_type, class label_type>
void QuadTree<number_type, label_type>::plot(Node<number_type,label_type> *p, std::vector<std::string> &board,
          int x_min, int x_max, int y_min, int y_max, bool draw_lines) const {
  // base case, draw nothing if this node is NULL
  if (p == NULL) return;
  // check that the dimensions range of this node make sense
  assert (x_min >= 0 && x_min <= x_max);
  assert (y_min >= 0 && y_min <= y_max);
  assert (board.size() >= y_max);
  assert (board[0].size() >= x_max);
  // verify that the point stored at this node fits on the board
  assert (p->pt.y >= 0 && p->pt.y < board.size());
  assert (p->pt.x >= 0 && p->pt.x < board[0].size());
  // draw the vertical and horizontal bars extending across the
  // range of this node
  if (draw_lines) {
    for (int x = x_min; x <= x_max; x++) {
      board[p->pt.y][x] = '-';
    }
    for (int y = y_min; y <= y_max; y++) {
      board[y][p->pt.x] = '|';
    }
  }
  // draw this label
  board[p->pt.y][p->pt.x] = p->label;
  // recurse on the 4 children
  plot(p->children[0],board,x_min ,p->pt.x-1,y_min ,p->pt.y-1,draw_lines);
  plot(p->children[1],board,p->pt.x+1,x_max ,y_min ,p->pt.y-1,draw_lines);
  plot(p->children[2],board,x_min ,p->pt.x-1,p->pt.y+1,y_max ,draw_lines);
  plot(p->children[3],board,p->pt.x+1,x_max ,p->pt.y+1,y_max ,draw_lines);
}


// ==============================================================

// prints all of the tree data with a pre-order (node first, then
// children) traversal of the tree structure

// driver function
template <class number_type, class label_type>
void QuadTree<number_type, label_type>::print_sideways() const { print_sideways(root_,""); }

// actual recursive function
template <class number_type, class label_type>
void QuadTree<number_type, label_type>::print_sideways(Node<number_type,label_type>* p, const std::string &indent) const {
  // base case
  if (p == NULL) return;
  // print out this node
  std::cout << indent << p->label << " (" << p->pt.x << "," << p->pt.y << ")" << std::endl;
  // recurse on each of the children trees
  // increasing the indentation
  print_sideways(p->children[0],indent+"  ");
  print_sideways(p->children[1],indent+"  ");
  print_sideways(p->children[2],indent+"  ");
  print_sideways(p->children[3],indent+"  ");
}

// ==============================================================
// ==============================================================


#endif
