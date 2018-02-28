#include <vector>
#include <iostream>
// ====================================================================================
// A Point is just a 2D coordinate.

class Point {
public:
  // constructor
  Point(int x_, int y_);
  // accessors
  int get_x() const { return x; }
  int get_y() const { return y; }
private:
  // representation
  int x;
  int y;
};

// helper function for printing Points
std::ostream& operator<<(std::ostream &ostr, const Point &p);
bool operator== (const Point &p1, const Point &p2);

// ====================================================================================
// An Edge is a vertical or horizontal line segment connecting 2 points.

class Edge {
public:
  // constructor
  Edge(const Point &a_, const Point &b_);
  // accessors
  const Point& get_a() const { return a; }
  const Point& get_b() const { return b; }
  int getNumBridges(){ return numBridges; }
  void decreaseNumBridges(){ numBridges-=1; }
  void setNumBridges(int numEdges_ ){ numBridges= numEdges_; }
private:
  // representation
  Point a;
  Point b;
  int numBridges;
};

// helper function for printing Edges
std::ostream& operator<<(std::ostream &ostr, const Edge &e);
bool operator== (const Edge &e1, const Edge &e2);
// ====================================================================================
// A Node is a Point with a count of the number of Edges that should touch it.

class Node {
public:
  Node(const Point &p, int n);
  // accessors
  const Point& get_pt() const { return pt; }
  void addConnectionNode(Node newNode);
  bool brokenPoint;
  std::vector<Node> getConnectionNodes() {return connectionNodes; }

  void increaseNumConnections(){
    
    currentConnections+=1;
    //std::cout<<currentConnections<<", "<<num_paths<<std::endl;
    if(currentConnections<=num_paths){
      brokenPoint=false;
    }
    else{
      brokenPoint=true;
    }
    //assert(currentConnections<=num_paths);
  }
  void setNumConnections(int newNum){ currentConnections=newNum; }
  int get_num_paths() const { return num_paths; }
  int getCurrentConections(){return currentConnections; }




private:
  Point pt;
  int num_paths;
  int currentConnections;
  std::vector<Node> connectionNodes;
  std::vector<Edge> edgeCombos;
};



// ====================================================================================
// A Graph is a collection of Nodes and Edges that can be printed.

class Graph {
public:
  // constructor
  Graph();
  
  //public member variables
  std::vector<Node> allConnectedNodes;
  bool solutionContinuous;
  
  // print helper functions
  
  //getters
  std::vector<Edge> const getEdges(){return edges; }
  std::vector<Edge> findAllEdges(std::vector<Edge> currentEdges);
  std::vector<Node> &getNodes(){ return nodes; }
  
  std::vector<Node> getSolutionConnections(Node &startNode, std::vector<Edge> solution);
  int sumOfNodes(){
    sumOfNodes_=0;
    for(int i=0; i<nodes.size();i++){
      sumOfNodes_+=nodes[i].get_num_paths();
    }
    return sumOfNodes_;
  }
  Node &getNodeAtPoint(Point targetPoint){
    for(int i=0; i<getNodes().size(); i++){
      if(getNodes()[i].get_pt()==targetPoint){
        return getNodes()[i];
      }
    }
    return getNodes()[0];
  }
  bool edgeCrosses(Edge e1, Edge e2);
  bool edgeGoesThroughPoint(Edge e);
  bool compareSolutions(std::vector<Edge> solution1, std::vector<Edge> solution2);
  //setters
  void addNode(const Point &p, int n);
  void addEdge(const Point &a, const Point &b);
  void printSolution() const;
  void printGraph() const;
private:
  // representation
  int max_x;
  int max_y;
  std::vector<Node> nodes;
  std::vector<Edge> edges;
  int sumOfNodes_;
};

// ====================================================================================


bool sortEdge(Edge e1, Edge e2);
