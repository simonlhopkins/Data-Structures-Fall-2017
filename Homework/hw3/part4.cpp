#include <iostream>
#include <cassert>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>

#include "the_stack.h"


// A few global constants
const intptr_t feet_per_mile = 5280;
const double seconds_between_samples = 4.25;


TheStack thestack;

/* Add your code here to finish the implementation. */
class GPSData{

public:
  void set_position(intptr_t newX, intptr_t newY){
    posX=newX;
    posY=newY;
  }
  intptr_t getX(){
    return posX;
  }
  intptr_t getY(){
    return posY;
  }
  void setSpeed(intptr_t newSpeed){
    speed=newSpeed;
  }
  intptr_t getSpeed(){
    return speed;
  }
  intptr_t getPaceSecs(){
    if(speed==0){
      return 0;
    }
    float paceF= 1/float(speed)*feet_per_mile;
    intptr_t min= int(paceF);
    intptr_t seconds= (paceF-min)*60;
    
    return seconds;
  }
  intptr_t getPaceMinutes(){
    if(speed==0){
      return 0;
    }
    float paceF= 1/float(speed)*feet_per_mile;
    intptr_t min= int(paceF);
    intptr_t seconds= (paceF-min)*60;
    
    return min;
  }

private:
  intptr_t speed=0;
  intptr_t posX;
  intptr_t posY;
  
};

//calculates and stores with each GPS sample the per-sample speed (in feet per minute) and
//the average speed over the whole path

void print(GPSData data[], intptr_t n){
  std::cout<<"  x-pos   y-pos   ft/min  min/mile"<<std::endl;
  for(intptr_t i=0; i<n;i++){
    std::cout<<std::setw(7)<<data[i].getX();
    std::cout<<std::setw(8)<<data[i].getY();
    std::cout<<std::setw(9)<<data[i].getSpeed();
    if(data[i].getPaceSecs()==0 && data[i].getPaceMinutes()==0){
      std::cout<<std::setw(10)<<"N/A";
    }
    else{
      std::cout<<std::setw(5)<<"";
    
      if(data[i].getPaceMinutes()<10){
        std::cout<<" "<<data[i].getPaceMinutes();
      }
      else{
        std::cout<<data[i].getPaceMinutes();
      }
      std::cout<<":";
      if(data[i].getPaceSecs()<10){
        std::cout<<"0"<<data[i].getPaceSecs();
      }
      else{
        std::cout<<data[i].getPaceSecs();
      }
    }
    
    std::cout<<std::endl;
  }

}

void print_pace(intptr_t pace){
  
  float tempPace=(1/float(pace))*feet_per_mile;
  intptr_t min= int(tempPace);
  intptr_t seconds= (tempPace-min)*60;
  if(min<10){
    std::cout<<" "<<min;
  }
  else{
    std::cout<<min;
  }
  std::cout<<":";
  if(seconds<10){
    std::cout<<"0"<<seconds;
  }
  else{
    std::cout<<seconds;
  }
  
  
}
float getTotalDistance(GPSData data[], intptr_t n){
  float tempTotalDistance=0;
  data[0].setSpeed(0);
  for(intptr_t i=1; i<n;i++){
    float tempDist;
    tempDist=sqrt(pow((data[i].getX()-data[i-1].getX()), 2)+
                              pow((data[i].getY()-data[i-1].getY()), 2));
    tempTotalDistance+= tempDist;
    data[i].setSpeed((tempDist/seconds_between_samples)*60);

  }
  return tempTotalDistance;
}
float distance(GPSData data[], intptr_t n, intptr_t &ave_feet_per_minute){
  thestack.set_label(&ave_feet_per_minute, "ave_feet_per_minute");
  thestack.set_label(&n, "n");

  intptr_t tempXValues[n];
  intptr_t tempYValues[n];
  for(intptr_t i=0; i<n; i++){
    tempXValues[i]=data[i].getX();
    tempYValues[i]=data[i].getY();
  }
  float totalDistance=0;
  data[0].setSpeed(0);
  float temp;
  for(intptr_t i=1; i<n; i++){
    temp=sqrt(pow((tempXValues[i]-tempXValues[i-1]), 2)+pow((tempYValues[i]-tempYValues[i-1]), 2));
    //temp=sqrt(pow((data[i].getX()-data[i-1].getX()), 2)+pow((data[i].getX()-data[i-1].getX()), 2));
    data[i].setSpeed((temp/seconds_between_samples)*60);
    totalDistance+=temp;
  }
  ave_feet_per_minute=(totalDistance*60)/((n-1)*seconds_between_samples);
  thestack.print();
  return totalDistance;
}

double filter(GPSData input[], GPSData output[], intptr_t n){
  output[0].set_position(input[0].getX(), input[0].getY());
  output[n-1].set_position(input[n-1].getX(), input[n-1].getY());
  for(intptr_t i=1; i<n-1;i++){
    GPSData tempGPSData;
    intptr_t tempX;
    intptr_t tempY;
    tempX=(input[i-1].getX()+input[i].getX()+input[i+1].getX())/3;
    tempY=(input[i-1].getY()+input[i].getY()+input[i+1].getY())/3;
    tempGPSData.set_position(tempX, tempY);
    output[i]=tempGPSData;
  }
  return(((getTotalDistance(input, n)-getTotalDistance(output, n))/getTotalDistance(input, n))*100);
}


// ==========================================================================================

void recursive_filter(GPSData* input, GPSData* output, intptr_t n, double percent_change_threshold) {
  // make a temporary array to store the intermediate filtering result
  GPSData tmp[n];

  // run the filter once
  double percent_change = filter(input,tmp,n);
  std::cout << "RECURSE " << std::setw(5) << std::fixed << std::setprecision(2)
            << percent_change << "% change" << std::endl;
  if (fabs(percent_change) < percent_change_threshold) {
    // if the percent change is under the target threshold, we're done
    for (intptr_t i = 0; i < n; i++) {
      // copy the data from the temporary arraya into the final answer
      output[i] = tmp[i];
    }
  } else {
    // otherwise, printptr_t the intermediate result (for debugging)
    // print(tmp,n);
    // and repeat!
    recursive_filter(tmp,output,n,percent_change_threshold);
  }
}

// ==========================================================================================

int main(int argc, char** argv) {

  // The mandatory first argument is the GPS data input file.
  if (argc < 2) {
    std::cerr << "ERROR!  This program must be called with at least one argument, the GPS data input file" << std::endl;
  }
  std::ifstream istr(argv[1]);
  assert (istr.good());


  // Load the GPS data file contents into a vector.
  // NOTE: this is the only place we use a vector in Part 1 of the homework.
  // This is just for convenience &/ flexibility for testing.
  std::vector<int> file_contents;
  intptr_t x;
  while (istr >> x) { file_contents.push_back(x); }


  // The optional second argument is the number of data points to work with.
  intptr_t count = -1;
  if (argc > 2) {
    count = atoi(argv[2]);
  }
  // If the argument is not provided, or if it is larger than the size
  // of the file, just use the whole file.
  if (count < 0 || int(file_contents.size()) / 2 < count) count = file_contents.size() / 2;

  
  // The optional third argument is the distance percentage change stopping criterion
  double percent_change_threshold = 0.1;
  if (argc > 3) {
    percent_change_threshold = atof(argv[3]);
  }  
  assert (percent_change_threshold > 0.0);

  
  // Prepare an array of the original data (the selected amount of data).
  GPSData input[count];
  for (intptr_t i = 0; i < count; i++) {
    std::string s;
    std::stringstream out;
    out << i;
    s = out.str();
    thestack.set_label((intptr_t*)&input[i], "input["+s+"]");
    input[i].set_position(file_contents[i*2],file_contents[i*2+1]);
  }
  thestack.print();

  // std::cout << "-----------------------------------" << std::endl;
  intptr_t original_avg_speed;
  double original_distance = distance(input,count,original_avg_speed) / double (feet_per_mile);
  thestack.set_label(&original_avg_speed, "original_avg_speed");

  // std::cout << "ORIGINAL" << std::endl;
  // print(input,count);


  // Prepare arrays for the filter data.
  GPSData filtered[count];
  GPSData recursive_filtered[count];

  
  // Perform a single pass of filtering
  // std::cout << "-----------------------------------" << std::endl;
  filter(input,filtered,count);
  intptr_t filtered_avg_speed;
  double filtered_distance = distance(filtered,count,filtered_avg_speed) / double (feet_per_mile);
  thestack.set_label(&filtered_avg_speed, "filtered_avg_speed");

  // std::cout << "FILTERED" << std::endl;
  // print(filtered,count);

  
  // Perform multiple passes of filtering (until the distance changes by less than the target percentage).
  // std::cout << "-----------------------------------" << std::endl;
  recursive_filter(input,recursive_filtered,count,percent_change_threshold);
  intptr_t recursive_filtered_avg_speed;
  thestack.set_label(&recursive_filtered_avg_speed, "recursive_filtered_avg_speed");

  double recursive_filtered_distance = distance(recursive_filtered,count,recursive_filtered_avg_speed) / double (feet_per_mile);
  // std::cout << "RECURSIVE FILTERED" << std::endl;
  // print(recursive_filtered,count);
  thestack.print();

  // Output
  // std::cout << "-----------------------------------" << std::endl;
  // std::cout << "ORIGINAL:           "
  //           << std::fixed << std::setprecision(2) << std::setw(5) << original_distance
  //           << " miles, " << std::setw(4) << original_avg_speed << " feet per minute, ";
  // print_pace(original_avg_speed);
  // std::cout << " minutes per mile" << std::endl;
  // std::cout << "FILTERED:           "
  //           << std::fixed << std::setprecision(2) << std::setw(5) << filtered_distance
  //           << " miles, " << std::setw(4) << filtered_avg_speed   << " feet per minute, ";
  // print_pace(filtered_avg_speed);
  // std::cout << " minutes per mile" << std::endl;  
  // std::cout << "RECURSIVE FILTERED: "
  //           << std::fixed << std::setprecision(2) << std::setw(5) << recursive_filtered_distance
  //           << " miles, " << std::setw(4) << recursive_filtered_avg_speed << " feet per minute, ";
  // print_pace(recursive_filtered_avg_speed);
  // std::cout << " minutes per mile" << std::endl;
  
  return 0;
}


