#include <fstream>
#include <cassert>
#include <cstdlib>
#include <iostream>

#include "graph.h"
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
/*

*/
bool operator== (const Point &pointA, const Point &pointB){
  return(pointA.get_x()==pointB.get_x() && pointA.get_y()==pointB.get_y());
}

Point findPath(const Point &startingPoint, const Point &endingPoint, std::vector<Point> &pathVector, Graph &g){


  if(startingPoint ==endingPoint){
    std::cout<<"ENDED at "<<endingPoint<<std::endl;
    std::cout<<"Path Vector that strait up got returned: "<<std::endl;
    std::cout<<"size: "<<pathVector.size()<<std::endl;
    std::cout<<"------------------"<<std::endl;
    for(int i=0; i<pathVector.size();i++){
      std::cout<<pathVector[i]<<std::endl;
    }
    std::cout<<"------------------"<<std::endl;
    return endingPoint;
  }
 
  //moves towards it in the x direction <---->
  if(startingPoint.get_y()==endingPoint.get_y()){
    Point tempPoint= startingPoint;

    Point newPoint= Point(startingPoint.get_x()+sgn(endingPoint.get_x()-startingPoint.get_x()),
                          startingPoint.get_y());
    std::cout<<tempPoint<<"->"<<newPoint<<std::endl;
    pathVector.push_back(newPoint);
    for(int i=0; i<g.getNodes().size(); i++){
      if(g.getNodes()[i].get_pt()==endingPoint){
        continue;
      }
      if(g.getNodes()[i].get_pt()==newPoint){
        std::cout<<"EXIT EARLY: on behalf of running into another node"<<std::endl;
        return pathVector;
      }
    }
    findPath(newPoint, endingPoint, pathVector, g);
  }
  //moves towards new point in y direction ^
  //                                       |
  //                                       |
  //                                       v
  else if(startingPoint.get_x()==endingPoint.get_x()){
    Point tempPoint= startingPoint;
    std::cout<<"starting at : "<<tempPoint<<" going ^ v \t"<<std::endl;

    Point newPoint= Point(startingPoint.get_x(),
                          startingPoint.get_y()+sgn(endingPoint.get_y()-startingPoint.get_y()));
    pathVector.push_back(newPoint);
    for(int i=0; i<g.getNodes().size(); i++){
      if(g.getNodes()[i].get_pt()==endingPoint){
        continue;
      }
      if(g.getNodes()[i].get_pt()==newPoint){
        std::cout<<"EXIT EARLY: on behalf of running into another node"<<std::endl;
       return pathVector;
      }
    }
    
    std::cout<<tempPoint<<"->"<<newPoint<<std::endl;
    findPath(newPoint, endingPoint, pathVector, g);
  }
  
  return pathVector;
  

    
}

int main(int argc, char* argv[] ) {


  // Parse the arguments
  if (argc < 2) {
    std::cout << "ERROR!  Must specify input file" << std::endl;
    exit(1);
  }
  std::ifstream istr(argv[1]);
  if (!istr.good()) {
    std::cout << "ERROR!  Could not open input file '" << argv[1] << "'" << std::endl;
    exit(1);
  }
  bool find_all_solutions = false;
  bool connected = false;
  for (int i = 2; i < argc; i++) {
    if (argv[i] == std::string("--find_all_solutions")) {
      find_all_solutions = true;
    } else if (argv[i] == std::string("--connected")) {
      connected = true;
    } else {
      std::cout << "ERROR!  Unknown argument '" << argv[i] << "'" << std::endl;
      exit(1);
    }
  }


  // Create an empty graph object
  Graph g;
  // Read in the puzzle from the input file
  int x,y,n;
  std::vector<Point> pts;
  while (istr >> x >> y >> n) {
    pts.push_back(Point(x,y));
    g.addNode(Point(x,y),n);
  }

  
  // The statements below hardcode the solution to puzzle1.txt.
  // You should remove this code and replace it with a general solver.
  if (argv[1] == std::string("puzzle1.txt")) {
    g.addEdge(pts[0],pts[1]);
    g.addEdge(pts[0],pts[1]);
    g.addEdge(pts[1],pts[2]);
    g.addEdge(pts[1],pts[2]);
    g.addEdge(pts[2],pts[3]);

    // print the edges (any order is ok)
    g.printSolution();
  }

  for(int i=0; i<g.getNodes().size();i++){
    //Node currentNode=g.getNodes()[i].get_pt();
    //std::cout<<g.getNodes()[i].get_pt()<<std::endl;
    for(int j=0; j<g.getNodes().size();j++){
      std::vector<Point> tempVec;
      std::cout<<"TRACING PATH FROM "<<g.getNodes()[i].get_pt()<<" -> "<<g.getNodes()[j].get_pt()<<std::endl;
      findPath(g.getNodes()[i].get_pt(),g.getNodes()[j].get_pt(), tempVec, g);
      std::cout<<"size of temp vec: "<<tempVec.size()<<std::endl;
      g.getNodes()[i].posConnections.push_back(tempVec);
      std::cout<<"size of pos connections: "<<g.getNodes()[i].posConnections.size()<<std::endl;

    }  
  }
  for(int i=0; i<g.getNodes().size();i++){
    std::cout<<"connections for "<<g.getNodes()[i].get_pt()<<": "<<std::endl;
    std::cout<<g.getNodes()[i].getPosConnections().size()<<std::endl;
  }




  // print a visualization of the (solved or unsolved) puzzle
  g.printGraph();




}



