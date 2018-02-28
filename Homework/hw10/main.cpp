#include <vector>
#include <iomanip>
#include <set>

#include "image.h"
#include "visualization.h"
#include "hashTable.h"


//./main.out -method simple -seed 10 images/marbles.pgm images/marbles_1.pgm images/marbles_2.pgm images/marbles_3.pgm
//./main.out -method hashtable -seed 10 images/marbles.pgm images/marbles_1.pgm images/marbles_2.pgm images/marbles_3.pgm
// ======================================================================
// Helper function to read the optional arguments and filenames from
// the command line.
void parse_arguments(int argc, char* argv[], 
                     std::string& method, int& seed, int& table_size, float& compare,
                     std::vector<std::string>& filenames) {
  for (int i = 1; i < argc; i++) {
    if (argv[i] == std::string("-method")) {
      i++;
      assert (i < argc);
      method = argv[i];
      assert (method == "simple" || method == "hashtable");
    } else if (argv[i] == std::string("-seed")) {
      i++;
      assert (i < argc);
      seed = atoi(argv[i]);
      assert (seed >= 1);
    } else if (argv[i] == std::string("-table")) {
      i++;
      assert (i < argc);
      table_size = atoi(argv[i]);
      assert (table_size >= 1);
    } else if (argv[i] == std::string("-compare")) {
      i++;
      assert (i < argc);
      compare = atof(argv[i]);
      assert (compare > 0.0 && compare <= 1.0);
    } else {
      filenames.push_back(argv[i]);
    }
  }
  assert (filenames.size() > 0);
}


// ======================================================================
// This simple algorithm is rather inefficient, and may not find the
// largest overlapping subregion.  But it will find a subregion match
// of size seed x seed, if one exists.
void SimpleCompare(const Image<int>& a, const Image<int>& b, 
                   Image<Color> &out, int which_color,int seed, float& percent,
                   std::vector<std::pair<BoundingBox,BoundingBox> >& regions) {

  // First, find a small seed that matches
  bool found = false;
  Point offset_a;
  Point offset_b;

  // Search over all possible points in image a
  for (int i = 0; i <= a.Width()-seed && !found; i++) {
    for (int j = 0; j <= a.Height()-seed && !found; j++) {
      // Search over all possible points in image b
      for (int i2 = 0; i2 <= b.Width()-seed && !found; i2++) {
        for (int j2 = 0; j2 <= b.Height()-seed && !found; j2++) {
          bool match = true;
          // Check for small seed match
          for (int x = 0; x < seed && match; x++) {
            for (int y = 0; y < seed && match; y++) {
              if (a.GetPixel(i+x,j+y) != b.GetPixel(i2+x,j2+y)) {
                //std::cout<<a.GetPixel(i+x,j+y)<<" != "<<b.GetPixel(i2+x,j2+y)<<std::endl;
                match = false;
              }
            }
          }
          if (match) {
            // break out of these loops!
            HighlightSeed(out,which_color,Point(i,j),seed);
            found = true;
            offset_a = Point(i,j);
            offset_b = Point(i2,j2);
          }
        }
      }
    }
  }
  if (!found) {
    // no match between these images
    percent = 0.0;
    return;
  } 

  int width = seed;
  int height = seed;
  

  //find the minimum width/height between the two images, so you don't go out of bounds for the smaller
  //image
  int minWith= std::min(a.Width()-offset_a.x-seed, b.Width()-offset_b.x-seed);
  int minHeight= std::min(a.Height()-offset_a.y-seed, b.Height()-offset_b.y-seed);
  for(int i=0; i<minWith-1; i++){
    //incriment width as long as it is within the bounds of the smaller image
    width++;
  }
  //incriment height as long as it is within the bounds of the smaller image
  for(int i=0; i<minHeight-1; i++){
    height++;
  }

  //set the bounding box of a and b from the images offset + the width and height
  BoundingBox bbox_a(offset_a,Point(offset_a.x+width,offset_a.y+height));
  BoundingBox bbox_b(offset_b,Point(offset_b.x+width,offset_b.y+height));
  regions.push_back(std::make_pair(bbox_a,bbox_b));
  // for the simple compare you can just compare the size of the bounding box to the size of the image
  percent = bbox_a.Width()*bbox_a.Height() / float (a.Width()*a.Height());
}

//this is the function that I call when you want to compare the images using hash tables
void HashCompare(const Image<int>& a, const Image<int>& b, 
                   Image<Color> &out, int which_color,int seed, float& percent,
                   std::vector<std::pair<BoundingBox,BoundingBox> >& regions, int tableSize, float compare) {
  
  //sets two hash tables to store values for image A and image B
  HashTable tableA(tableSize);
  HashTable tableB(tableSize);

  //boolean that keeps track if it found any similarities, because if it did not, it should just return
  //preventing any additional computation
  bool found=false;
  //loops through image 1, inserting it into the hash table for imahe A
  for(int i=0; i<a.Width()-seed-1; i++){
    for(int j=0; j<a.Height()-seed-1; j++){
      std::vector<int> v;
      for (int x = 0; x < seed; x++) {
        for (int y = 0; y < seed; y++) {
          v.push_back(a.GetPixel(i+x, j+y));
        }
      }
      //hashes the vector of pixels as large as the seed originating from the point i,j
      int key= tableA.hash(v);
      tableA.insert(key, Point(i,j));
    }
  }

  std::set<std::pair<Point, int>, sortOrder> allValuesA;
  
  
  for(int i=0; i<b.Width()-seed-1; i++){
    for(int j=0; j<b.Height()-seed-1; j++){
      std::vector<int> v;
      for (int x = 0; x < seed; x++) {
        for (int y = 0; y < seed; y++) {
          v.push_back(b.GetPixel(i+x, j+y));
        }
      }
      //hashes the vector of pixels as large as the seed originating from the point i,j
      int key= tableB.hash(v);
      tableB.insert(key, Point(i,j));
      //if there was a value in table A at the index it just was inserted to, it loops through
      //all of the values at that index, and checks if the point is actually that point, or if
      //it was just a coincidence
      if(!(tableA.getTable()[key%tableSize].empty())){
        //loops through the values at [key%tableSize]
        for(std::list<std::pair<Point, int> >::iterator itr= tableA.getTable()[key%tableSize].begin();
            itr != tableA.getTable()[key%tableSize].end(); itr++){
          //if the second value, which is just the raw hash value, is equal, then that means that both
          //the hash % tablesize is equal and hash==hash, meaning that it is truly a match
          if(itr->second==std::hash<int>()(key)){
            found=true;
            //insert it into a set that is ordered by the hash value, if I just drew here 1% of the time
            //since it is looping over in a geometric pattern, I would not get the random assortment that
            //I need to more accurately calculate the bounding boxes
            allValuesA.insert(*itr);
            
            

          }
        }

      }
    }
  }

  //if no overlaps happen just return with 0% found
  if(!found){
    percent=0.0;
    return;
  }

  //this count keeps track of how many of the matches you've been through
  int count=0;
  //this keeps track of how many values were actually added
  int hits=0;
  

    
  //these two sets are used to calculate the bounding boxes for A and B, which contain only
  //the actual drawn values as perameters
  std::set<std::pair<Point, int>, sortOrder> addedBoxesA;
  std::set<std::pair<Point, int>, sortOrder> addedBoxesB;
  //loops through all of the total found values
  for(std::set<std::pair<Point, int> >::iterator itr= allValuesA.begin();
      itr!= allValuesA.end(); itr++){
    //inserts a certain percent of them
    if(count % int(1/compare) ==0){

      Point currentPoint= itr->first;
      //incriment the amount of values added
      hits++;
      //highlight the value
      HighlightSeed(out,which_color,currentPoint,seed);
      //add it to the boxes that were added in A
      addedBoxesA.insert(*itr);
      //find corresponding b value
      std::pair<Point, int> bValue= tableB.find(*itr, itr->second);
      addedBoxesB.insert(bValue);
      
    }
    count++;
  }
  int maxXA= addedBoxesA.begin()->first.x;
  int maxYA= addedBoxesA.begin()->first.y;
  int minXA= addedBoxesA.begin()->first.x;
  int minYA= addedBoxesA.begin()->first.y;

  //finds the minimum and maximum A value
  for(std::set<std::pair<Point, int> >::iterator itr= addedBoxesA.begin();
      itr!= addedBoxesA.end(); itr++){
    if(itr->first.x>maxXA){
      maxXA=itr->first.x;
    }
    if(itr->first.y>maxYA){
      maxYA=itr->first.y;
    }
    if(itr->first.x<minXA){
      minXA=itr->first.x;
    }
    if(itr->first.y<minYA){
      minYA=itr->first.y;
    }
  }
  int maxXB= addedBoxesB.begin()->first.x;
  int maxYB= addedBoxesB.begin()->first.y;
  int minXB= addedBoxesB.begin()->first.x;
  int minYB= addedBoxesB.begin()->first.y;
  //finds the minimum and maximum B value

  for(std::set<std::pair<Point, int> >::iterator itr= addedBoxesB.begin();
      itr!= addedBoxesB.end(); itr++){
    if(itr->first.x>maxXB){
      maxXB=itr->first.x;
    }
    if(itr->first.y>maxYB){
      maxYB=itr->first.y;
    }
    if(itr->first.x<minXB){
      minXB=itr->first.x;
    }
    if(itr->first.y<minYB){
      minYB=itr->first.y;
    }
  }


  BoundingBox bbox_a(Point(minXA, minYA), Point(maxXA+seed, maxYA+seed));
  BoundingBox bbox_b(Point(minXB, minYB), Point(maxXB+seed, maxYB+seed));
  regions.push_back(std::make_pair(bbox_a,bbox_b));

  //the percent can then be found by taking the total amount of hits divided by the total amount
  //of pixels that you loop through, which is (a.Width()-seed)*(a.Height()-seed). Since the hits variable
  //only resents 1% of the total pixels you must also only divide by 1% of the area, whcuh is where the
  //* compare comes from
  percent = hits / (float ((a.Width()-seed)*(a.Height()-seed))*compare);
  
  
}

// ======================================================================


int main(int argc, char* argv[]) {

  // default command line argument values
  std::string method = "simple";
  int seed = 5;
  int table_size = 1000000;
  float compare = 0.05;
  std::vector<std::string> filenames;
  parse_arguments(argc,argv,method,seed,table_size,compare,filenames);


  // Load all of the images
  std::vector<Image<int> > images(filenames.size());
  for (int i = 0; i < filenames.size(); i++) {
    images[i].Load(filenames[i]);
  }

  // Loop over all input images, comparing to every other input image
  for (int a = 0; a < filenames.size(); a++) {
    std::cout << filenames[a] << std::endl;
    // prepare a color visualization image for each input file
    Image<Color> out;
    InitializeOutputFile(images[a],out);
    int which_color = -1;
    for (int b = 0; b < filenames.size(); b++) {
      if (a == b) continue;
      which_color++;
      
      // Find the one (or more!) subregions that match between this pair of images
      std::vector<std::pair<BoundingBox,BoundingBox> > regions;
      float percent = 0.0;
      if (method == "simple") {
        SimpleCompare(images[a],images[b],out,which_color,seed,percent,regions);
      } else {
        assert (method == "hashtable");
        HashCompare(images[a],images[b],out,which_color,seed,percent,regions, table_size, compare);

      } 

      std::cout << std::right << std::setw(7) << std::fixed << std::setprecision(1) 
                << 100.0 * percent << "% match with " << std::left << std::setw(20) << filenames[b];

      for (int i = 0; i < regions.size(); i++) {
        std::cout << "   " << regions[i].first << " similar to " << regions[i].second;
        // add the frame data to the visualization image
        DrawBoundingBox(out,regions[i].first,which_color);
      }
      std::cout << std::endl;
      // Save the visualization image
      std::string f = "output_" + filenames[a].substr(0,filenames[a].size()-4) + ".ppm";
      out.Save(f);
    }
  }
}
