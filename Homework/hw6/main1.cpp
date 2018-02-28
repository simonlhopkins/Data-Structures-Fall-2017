#include <fstream>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <algorithm>

#include "graph.h"


//== operator to compare a node to a node, makinf it not matter where the a and b are
bool operator== (const Node &n1, const Node &n2){
  return(n1.get_pt().get_x()==n2.get_pt().get_x() && n1.get_pt().get_y()==n2.get_pt().get_y());
}

//recursive function that returns nothing, but takes in a vector of the current edges set on the
//graph, a copy of the graph, and a reference variable made in main that holds all the viable
//solutions for the graph.
void solve(std::vector<Edge> current, Graph g, std::vector<std::vector<Edge> > &allSolutions){
  //sets a vector options to be a vector of all of the possible places an edge could exist
  std::vector<Edge> options= g.findAllEdges(current);
  //if options is 0, that means its either solved or at an impossible solution.
  if(options.size()==0){
    //if the size of current is the sum of all the nodes/2, then it is solved
    if(current.size()== (g.sumOfNodes()/2)){
      //sort the edges using custom sort function to make them uniform so I know if one is a duplicate
      std::sort(current.begin(), current.end(), sortEdge);
      
      bool alreadyExists= false;
      //if it compare solutions, which returns true if they are equal, is true, then its a duplicate solution
      for(int i=0; i<allSolutions.size();i++){
        if(g.compareSolutions(current, allSolutions[i])){
          alreadyExists=true;
        }
      }
      //if it doesn't exsist, print out the length and add it to allsolutions
      if(!alreadyExists){
        std::cout<<"solution is this long: "<<current.size()<<std::endl;

        allSolutions.push_back(current);
      }

    } 
  }
  else{
    //loops through all of the possible options for putting down a bridge
    for(int i=0; i<options.size();i++){
      //makes a new vector that is the same as current and adds the ith option to it
      std::vector<Edge> newCurrent= current;
      newCurrent.push_back(options[i]);
      //makes a copy of graph
      Graph newGraph= g;
      //incriments the number of connections on newgraph at the endpoints of the bridge
      newGraph.getNodeAtPoint(options[i].get_a()).increaseNumConnections();
      newGraph.getNodeAtPoint(options[i].get_b()).increaseNumConnections();
      //I was getting a weird bug where even if a node's connections was over the max connections
      //it would still list iit as a viable node. This broken point variable brute forces it and 
      //is true if the connections are greater than the max number of connections.
      if(!(newGraph.getNodeAtPoint(options[i].get_a()).brokenPoint || newGraph.getNodeAtPoint(options[i].get_b()).brokenPoint)){
        //solves on the new graph and current
        solve(newCurrent, newGraph, allSolutions);
      }

      
    }
  }
  
}

//recursive function that starts at one point, and checks if it is connected to
//all of the other points, i.e. the entire graph's solution is connected
void solutionIsConnected(Node startNode, std::vector<Edge> solution, Graph &g){
  //makes a vector of all of the nodes connected to the start node
  std::vector<Node> connectedToStart = g.getSolutionConnections(startNode, solution);
  //adds all the viable nodes to connected to start
  for(int i=0; i<connectedToStart.size();i++){
    g.allConnectedNodes.push_back(connectedToStart[i]);
  }
  //for each of those it calls solutionIsConnected, which modifies allConnectedNodes. I can then check
  //if allConnectedNodes.size() is equal to the total number of nodes, meaning that it is all connected
  for(int i=0; i<connectedToStart.size();i++){
    solutionIsConnected(connectedToStart[i], solution, g);
  }

}
int main(int argc, char* argv[] ) {
  //vector that holds a vector of all the solution vectors
  std::vector<std::vector<Edge> > allSolutions;
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
  bool oneSolution= true;
  for (int i = 2; i < argc; i++) {
    if (argv[i] == std::string("--find_all_solutions")) {
      find_all_solutions = true;
      oneSolution=false;
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
  //nested for loops that get all of the connecting nodes for every point on the graph and sets them
  for(int i=0; i<g.getNodes().size(); i++){
    for(int j=0; j<g.getNodes().size(); j++){
      if(g.getNodes()[i].get_pt().get_x()==g.getNodes()[j].get_pt().get_x() || 
          g.getNodes()[i].get_pt().get_y()==g.getNodes()[j].get_pt().get_y()){
        if(g.getNodes()[i]==g.getNodes()[j]){ continue; }
        g.getNodes()[i].addConnectionNode(g.getNodes()[j]);
      }
    }
  }
  //so far the solution is nothing at the start
  std::vector<Edge> solution;

  //calls the solve recursive function starting with the empty solution
  solve(solution, g, allSolutions);
  //vector of graphs that have solved solutions.
  std::vector<Graph> solvedGraphs;
  //loops through all the solutions and makes a graph with the solved edges and adds it to solved graphs
  for(int i=0; i<allSolutions.size(); i++){
    Graph tempGraph = g;
    for(int j=0; j<allSolutions[i].size(); j++){
      tempGraph.addEdge(allSolutions[i][j].get_a(), allSolutions[i][j].get_b());

    }
    //recursively goes through all the points and sets the graph variable solutionContinuous if it is continuous
    solutionIsConnected(tempGraph.getNodes()[0], allSolutions[i], tempGraph);
    tempGraph.solutionContinuous = (tempGraph.allConnectedNodes.size()==tempGraph.getNodes().size());
      //make some variable false
    
    solvedGraphs.push_back(tempGraph);
  }
  
  int counter=0;
  
  //prints out the graphs depending on the command line arguments
  for(int i=0; i<solvedGraphs.size(); i++){
    if((!connected || solvedGraphs[i].solutionContinuous) && (!oneSolution || (counter==0))){
      counter++;
      solvedGraphs[i].printSolution();
      solvedGraphs[i].printGraph();
    }
    
  }
  if(counter==0){
    std::cout<<"No solutions"<<std::endl;
    return 0;
  }
  if(!oneSolution){
    std::cout<<"Found "<<counter<<" solution(s)"<<std::endl;

  }
  return 0;
  



}
