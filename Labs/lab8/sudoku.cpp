#include <vector>
#include <set>
#include <iostream>
#include <math.h>
#include <cassert>
#include "sudoku.h"

using std::set;
using std::vector;
using std::cout;
using std::cin;
using std::endl;

// ======================================================
// A program to solve very simple Sudoku puzzles
// ======================================================

int main() {

  int size;
  while (cin >> size) {
    // Read in the board size and create an empty board
    Sudoku board(size);

    // Read in and set the known positions in the board
    char c;
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {      
        cin >> c;
        if (c != '.') {
          board.Set(i,j,c-'0');
        }
      }
    }
    board.Print();
    // The iterative solving routine 
    while (1) {
      
      // Is the puzzle is already solved?
      if (board.IsSolved()) {
        cout << "\nPuzzle is solved:" << endl;
        board.Print();
        break;
      }
      
      // Is the puzzle impossible?
      if (board.Impossible()) {
        cout << "\nCannot solve this puzzle:" << endl;
        board.Print();
        break;
      }
      
      // Try to reduce the number of choices in one or more board cells
      // by propagating known values within each row, column, and quadrant
      int changed = 0;
      for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
          if (board.KnownValue(i,j))
            changed += board.Propagate(i,j);
        }
      }
      
      // If nothing changed this iteration, give up
      if (changed == 0) {
        cout << "\nPuzzle is too difficult for this solver!" << endl;
        board.Print();
        break;      
      }
    }
  }
}

// ======================================================

// Construct an empty board
Sudoku::Sudoku(int s) {
  // set the size
  size = s;
  quadrant_size = (int)sqrt(size);
  assert (quadrant_size*quadrant_size == s);
  for(int i=0; i< s; i++){
    board.push_back(std::vector<std::set<int> >)
    for(int j=0; j<s;j++){
      
    }
  }
  for(int i=0; i < s; i++){
    //board[i].push_back(std::vector<std::set<int> >());
    for(int j=0; j< s; j++){
      std::set<int> newSet;
      for(int n=0; n < s; n++){
        newSet.insert(n);
      }
      //board[j].push_back(newSet);
      board[i][j]=newSet;
    }
    
  }
  
  // You need to finish this function!


}

// To construct the puzzle, set the value of a particular cell
void Sudoku::Set(int i, int j, int value) {
  // make sure the value is one of the legal values
  assert (board[i][j].find(value) != board[i][j].end());
  board[i][j].clear();
  board[i][j].insert(value);
}

// Does the cell have exactly one legal value?
bool Sudoku::KnownValue(int i, int j) const {
  return (board[i][j].size() == 1);
}

// If there is exactly one number in each cell of the grid, the puzzle is solved!
bool Sudoku::IsSolved() const {
  for(int i=0; i<size; i++){
    for(int j=0; j<size; j++){
      if(board[i][j].size()!=1){
        return false;
      }
    }
  }
  return true;

  // You need to write this function


}

// If there are no legal choices in one or more cells of the grid, the puzzle is impossible
bool Sudoku::Impossible() const {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (board[i][j].size() == 0) 
        return true;
    }
  }
  return false;
}

// print out the solution
void Sudoku::Print() const {
  int solved = IsSolved();
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      int count = size+1;
      for (set<int>::iterator itr = board[i][j].begin(); itr != board[i][j].end(); itr++) {
        count--;
        cout << *itr;
      }
      if (!solved) {
        // if it's not solved, print out spaces to separate the values
        for (int k = 0; k < count; k++) {
          cout << " ";
        }
      }
    }
    cout << endl;
  }
}


// Propagate information from this cell along columns & rows &
// within quadrant. Return the number of cells that changed.
int Sudoku::Propagate(int i, int j) {

  int counter=0;

  std::set<int>::iterator currentValue= board[i][j].begin();
  // You need to write this function

  //checks if it is in same column
  for(int row=0; row<size; row++){
    if(board[row][j].find(*currentValue)!=board[row][j].end()){
      board[row][j].erase(currentValue);
      counter++;
    }
  }
  //check same row
  for(int column=0; column<size; column++){
    if(board[i][column].find(*currentValue)!=board[i][column].end()){
      board[i][column].erase(currentValue);
      counter++;
    }
  }

  int quadX= i-(i%quadrant_size);
  int quadY= j- (j%quadrant_size);

  for(int i=0; i<quadrant_size; i++){
    for(int j=0; j<quadrant_size; j++){
      if(board[quadX+i][quadY+j].find(*currentValue)!=board[quadX+i][quadY+j].end()){
        board[quadX+i][quadY+j].erase(currentValue);
        counter++;
      }
    }
  }




  return counter;

}
