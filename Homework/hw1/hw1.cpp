#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <cstring>
#include <iomanip>
#include <cmath>
#include <cstdlib>


// ======================================================================================

// Helper function to read the provided font from a file.  The format
// of the font file is described in comments below.  The width,
// height, and bitmap_letters variables are set by this function.

void ReadFont(const std::string &font_file, 
	      int &width,
	      int &height,
	      std::vector<std::vector<std::string> > &bitmap_letters) {

  // open the font file for reading
  std::ifstream istr(font_file.c_str());
  if (!istr) { 
    std::cerr << "ERROR: cannot open font file " << font_file << std::endl; 
    exit(0);
  }

  // read in the width & height for every character in the file
  istr >> width >> height;
  assert (width >= 1);
  assert (height >= 1);

  // Create a vector to store all 256 ASCII characters of the
  // characters.  Each character is represented as a vector of
  // <height> strings that are each <width> wide.  Initially the
  // characters are unknown (represented with the '?' character).
  bitmap_letters = std::vector<std::vector<std::string> > 
    ( 256, std::vector<std::string> ( height, std::string(width, '?')));

  // read in all the characters
  // first is the ascii integer representation of the character
  int ascii;
  while (istr >> ascii) {
    assert (ascii >= 0 && ascii < 256);
    // next the character is printed in single quotes
    char c;
    istr >> c;
    assert (c == '\'');
    // use std::noskipws to make sure we can read the space character correctly
    istr >> std::noskipws >> c;
    // verify that the ascii code matches the character
    assert (c == (char)ascii);
    // switch back to std::skipws mode
    istr >> std::skipws >> c;
    assert (c == '\'');
    // read in the letter
    std::vector<std::string> bitmap;
    std::string tmp;
    for (int i = 0; i < height; i++) {
      istr >> tmp;
      assert ((int)tmp.size() == width);
      // make sure the letter uses only '#' and '.' characters
      for (unsigned int j = 0; j < tmp.size(); j++) { 
        assert (tmp[j] == '.' || tmp[j] == '#'); 
      }
      bitmap.push_back(tmp);
    }
    // overwrite the initially unknown letter in the vector
    bitmap_letters[ascii] = bitmap;
  }
}

//this is a helper function that replaces all of the 'originalLetter' with 'newLetter'
//in 'oldword'
std::string replaceLetter(std::string oldWord, char originalLetter, char newLetter){
  std::string newWord=oldWord;
  //loops through oldword and if original letter occurs, it is replaces with new letter
  for(int i=0; i<oldWord.length(); i++){
    if(newWord[i]==originalLetter){
      newWord[i]=newLetter;
    }
  }
  return newWord;


}


int main(int argc, char *argv[]){
  int letterWidth;
  int letterHeight;
  std::vector<std::vector<std::string> > letterBitmap;
  if(std::strcmp("read", argv[1])==0){
    std::vector<char> allCharVec;
    ReadFont(argv[2], letterHeight, letterWidth, letterBitmap);

    char currentChar;
    std::ifstream myfile (argv[3],std::fstream::in);
    std::string firstLine;
    std::getline(myfile, firstLine);
    int numLetters=firstLine.length()/letterWidth;
    std::vector<std::vector<std::string> > lettersFromFile(numLetters);

    std::cout<<lettersFromFile.size()<<std::endl;

    while (myfile >> std::noskipws >> currentChar) {
      for(int i=0; i<letterWidth;i++){
        
      }
      allCharVec.push_back(currentChar);
    }
    std::cout<<allCharVec.size()/letterWidth<<std::endl;
    
    // for(int i=0; i<allCharVec.size();i++){
    //   int currentRow=(i+firstLine.length())%firstLine.length();
    //   std::cout<<currentRow/(letterWidth)<<"'"<<allCharVec[i]<<"'"<<std::endl;
    //   //lettersFromFile[j].push_back(numLetters[i]);
      
    // }
    for(int i=0; i<allCharVec.size();i++){
      
      std::cout<<allCharVec.size()<<std::endl;
      //lettersFromFile[j].push_back(numLetters[i]);
      
    }
    

    return 0;
  }
  //throw statement for if there are not enough arguments given
  // if(argc<6){
  //   std::cerr<<"ERROR. expected 6 arguments...got: "<<(argc)<<std::endl;
  //   return 1;
  // }
  //declare letterWidth, letterHeight, and letterBitmap but do not initialize them because they will be
  //set to a value inside the ReadFont function
  
  //Call ReadFont which sets all of the variables using the text file given as the first argument
  ReadFont(argv[2], letterWidth, letterHeight, letterBitmap);
  //sets the number of chars in the word to wordLength
  int wordLength=strlen(argv[3]);
  //creates a vector of vectors of strings that will only contain the vectors of strings for
  //the letters given in the command line argument
  std::vector<std::vector<std::string> > wordVector;
  //loops through the word given as a command line argument and adds the vector of strings to word vector
  //for each letter
  std::cout<<argv[2]<<std::endl;
  





  for(int i=0; i<wordLength; i++){
    wordVector.push_back(letterBitmap[argv[3][i]]);
  }
  //loops through all the lines for each letter
  for(int i=0; i<letterHeight;i++){
    //prints out each line for all of the words, then moves onto the next line for all of the
    //words for letterHeight amount of times
    for(int j=0; j<wordLength;j++){
      //sets the string to be printef to a temperary value adding an extra period for spacing
      std::string tempWord=wordVector[j][i]+".";
      //using replace letter it replaces all of the '.' with the 5th command line argument
      //and replaces the '#' with the 4th command line argument
      std::cout<<replaceLetter(replaceLetter(tempWord, '.', argv[5][0]),'#', argv[4][0]);
    }
    //prints a newline after all of one line for all letts are printed
    std::cout<<std::endl;
  }
  return 0;
}


// ======================================================================================