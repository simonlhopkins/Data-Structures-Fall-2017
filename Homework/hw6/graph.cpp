#include <string>
#include <iostream>
#include <cassert>
#include <algorithm>
#include "graph.h"


// ====================================================================================
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
Point::Point(int x_, int y_) : x(x_),y(y_) {
  // Points must have positive coordinates
  assert (x_ >= 0);
  assert (y_ >= 0);
}

// helper function for printing Points
std::ostream& operator<<(std::ostream &ostr, const Point &p) {
  ostr << "(" << p.get_x() << "," << p.get_y() << ")";
  return ostr;
}
//== operator for point
bool operator== (const Point &p1, const Point &p2){
  if(p1.get_x()==p2.get_x() && p1.get_y()== p2.get_y()){ return true; }
  return false;
}


// ====================================================================================

Edge::Edge(const Point &a_, const Point &b_) : a(a_),b(b_) {
  // Edges must be horizontal or vertical
  assert ((a_.get_x() == b_.get_x() && a_.get_y() != b_.get_y()) ||
          (a_.get_x() != b_.get_x() && a_.get_y() == b_.get_y()));
}


// ====================================================================================

Node::Node(const Point &p, int n) : pt(p),num_paths(n),currentConnections(0) {
  // Nodes/islands must have at least one Edge/bridge.  At most 2
  // Edges/bridges can connect a pair of Nodes/islands.  Edges/bridges
  // may only run horizontal or vertical and cannot cross other
  // Edges/bridges.  This a Node/island can support at most 8 bridges.
  assert (n >= 1 && n <= 8);
}

//adds a Node to the connection nodes
void Node::addConnectionNode(Node newNode){
  connectionNodes.push_back(newNode);
}

// ====================================================================================

// helper function for printing Edges
std::ostream& operator<<(std::ostream &ostr, const Edge &e) {
  ostr << e.get_a() << "," << e.get_b();
  return ostr;
}

bool operator== (const Edge &e1, const Edge &e2){
  if(e1.get_a()==e2.get_a() && e1.get_b()==e2.get_b()){
    return true;
  }
  if(e1.get_a()==e2.get_b() && e1.get_b()==e2.get_a()){
    return true;
  }
  return false;
}
//checks if a node is in a list, helper for getSolutionConnections
bool isNodeInList(Node targetNode, std::vector<Node> currentNodes){
  for(int i=0; i<currentNodes.size();i++){
    if(targetNode.get_pt()==currentNodes[i].get_pt()){
      return true;
    }
  }
  return false;
}
//function that returns all of the viable nodes that connect to a start node via bridges
std::vector<Node> Graph::getSolutionConnections(Node &startNode, std::vector<Edge> solution){
  //makes a vector that will hold all the nodes it will go to
  std::vector<Node> connectingNodes;
  for(int i=0; i<solution.size(); i++){
    if(startNode.get_pt()==solution[i].get_a()){
      if(!isNodeInList(getNodeAtPoint(solution[i].get_b()), allConnectedNodes) && !isNodeInList(getNodeAtPoint(solution[i].get_b()), connectingNodes)){
        connectingNodes.push_back(getNodeAtPoint(solution[i].get_b()));
      }
    }
    else if(startNode.get_pt()==solution[i].get_b()){
      if(!isNodeInList(getNodeAtPoint(solution[i].get_a()), allConnectedNodes) && !isNodeInList(getNodeAtPoint(solution[i].get_a()), connectingNodes)){
        connectingNodes.push_back(getNodeAtPoint(solution[i].get_a()));
      }
    }
  }
  return connectingNodes;
}

// ====================================================================================

Graph::Graph() {
  // We track the maximum dimensions of the graph.
  sumOfNodes_=0;
  max_x = 0;
  max_y = 0;
}


void Graph::addNode(const Point &p, int n) {
  // Incrementally track the maximum dimensions of the graph.
  max_x = std::max(p.get_x(),max_x);
  max_y = std::max(p.get_y(),max_y);
  nodes.push_back(Node(p,n));
}

//helper function that checks if edge 1 intersects with edge 2 at any point
bool Graph::edgeCrosses(Edge e1, Edge e2){
  
  int e1x1 = std::max(e1.get_a().get_x(), e1.get_b().get_x());
  int e1x2 = std::min(e1.get_a().get_x(), e1.get_b().get_x());
  int e1y1 = std::max(e1.get_a().get_y(), e1.get_b().get_y());
  int e1y2 = std::min(e1.get_a().get_y(), e1.get_b().get_y());
  

  int e2x1 = std::max(e2.get_a().get_x(), e2.get_b().get_x());
  int e2x2 = std::min(e2.get_a().get_x(), e2.get_b().get_x());
  int e2y1 = std::max(e2.get_a().get_y(), e2.get_b().get_y());
  int e2y2 = std::min(e2.get_a().get_y(), e2.get_b().get_y());
  //vertical line e1      horizontal on e2
  if(e1x1==e1x2){
    if(e2x1==e2x2){
      return false;
    }

    if(e1y1> e2y2 && e1y2< e2y2){
      if(e1x1 > e2x2 && e1x1 < e2x1){
        return true;
      }
    }
  }
  //horizontal line e1     verical e2
  else{
    if(e2y1== e2y2){
      return false;
    }

    if(e1x1 > e2x1 && e1x2 < e2x1){
      if(e1y1 < e2y1 && e1y1 > e2y2){
        return true;
      }
    }
  }
  return false;
}

void Graph::addEdge(const Point &a, const Point &b) {
  assert (a.get_x() <= max_x);
  assert (a.get_y() <= max_y);
  assert (b.get_x() <= max_x);
  assert (b.get_y() <= max_y);
  Edge newEdge=Edge(a,b);

  edges.push_back(newEdge);
}



/*
This function will look at the graph, and find all of the viable connections that you can make with the 
condition that they are legal if all of the current edges exsist. This is the main helper function for the
recursion because in each loop, you need to know what connections are legal, so you don't chec extraineous
edges
*/
std::vector<Edge> Graph::findAllEdges(std::vector<Edge> currentEdges){
  //all the possible edges
  std::vector<Edge> posEdges;
  //loops through all of the nodes
  for(int i=0; i<nodes.size(); i++){
    //if a node is full of bridges
    if(nodes[i].getCurrentConections()>=nodes[i].get_num_paths()){
      continue;
    }

    //loops through the connecting nodes for each node
    for(int j=0; j< nodes[i].getConnectionNodes().size(); j++){
      //makes a new edge between the node and its jth connection
      Edge newEdge = Edge(nodes[i].get_pt(), nodes[i].getConnectionNodes()[j].get_pt());
      
      bool validEdge=true;
      //checks if the bridge goes through a point
      if(edgeGoesThroughPoint(newEdge)){
        validEdge=false;
        continue;
      }
      //checks if the jth node is full
      if(nodes[i].getConnectionNodes()[j].getCurrentConections()>=nodes[i].getConnectionNodes()[j].get_num_paths()){
        validEdge=false;

        continue;
      }

      //this counter tracks how many times a value exsists in the list, a value can only exist up to two times

      int counter=0;
      //loops through all the current edges
      for(int k=0; k<currentEdges.size();k++){
        //if the new edge crosses the new edge it is illegal
        if(edgeCrosses(newEdge, currentEdges[k])){
          validEdge=false;
        }
        //if the edge exsists in the current edges, incriment the counter
        if(newEdge==currentEdges[k]){
          ++counter;
        }


      }
      //if it is two or greater, it is illegal
      if(counter>=2){
        continue;
        validEdge=false;
      }
      //loops through the possible edges to remove duplicates
      for(int k=0; k<posEdges.size(); k++){
        if(newEdge==posEdges[k]){
          validEdge=false;
        }
      }


      if(validEdge){
        //if that edge can exist then add it to possible edges
        posEdges.push_back(newEdge);
      }
    }
    
  }
  return posEdges;
  
}

//function should return true if the edge e goes through any point on the graph.
bool Graph::edgeGoesThroughPoint(Edge e){
  int ex1 = std::max(e.get_a().get_x(), e.get_b().get_x());
  int ex2 = std::min(e.get_a().get_x(), e.get_b().get_x());
  int ey1 = std::max(e.get_a().get_y(), e.get_b().get_y());
  int ey2 = std::min(e.get_a().get_y(), e.get_b().get_y());
  for(int i=0; i<getNodes().size(); i++){
    //vertical line
    if(ex1==ex2){
      if(getNodes()[i].get_pt().get_x()==ex1){
        if(getNodes()[i].get_pt().get_y()<ey1 && getNodes()[i].get_pt().get_y()>ey2){
          return true;
        }
      }
    }
    if(ey1==ey2){
      if(getNodes()[i].get_pt().get_y()==ey1){
        if(getNodes()[i].get_pt().get_x()<ex1 && getNodes()[i].get_pt().get_x()>ex2){
          return true;
        }
      }
    }
  }
  return false;
}

//return true if they are equal
bool Graph::compareSolutions (std::vector<Edge> solution1, std::vector<Edge> solution2){
  for(int i=0; i<solution1.size(); i++){
    if(!(solution1[i]==solution2[i])){
      return false;
    }
  }
  return true;
}
void Graph::printSolution() const {
  std::cout << "Solution:" << std::endl;
  for (int i = 0; i < edges.size(); i++) {
    std::cout << edges[i] << std::endl;
  }
}



void Graph::printGraph() const {
  // a graph should have at least one node/island
  assert (nodes.size() > 1);
  // the dimensions of the board should be positive
  assert (max_x >= 0);
  assert (max_y >= 0);

  // allocate an empty/blank board, twice as big in each dimension so
  // we can draw lines between the nodes/islands.
  std::vector<std::string> board(2*max_y+1,std::string(2*max_x+1,' '));
  // mark all of the islands with their bridge count
  for (int i = 0; i < nodes.size(); i++) {
    const Point &pt = nodes[i].get_pt(); 
    std::string value = std::to_string(nodes[i].get_num_paths());
    board[2*pt.get_y()][2*pt.get_x()] = value[0];
  }

  // loop over all of the edges/bridges
  for (int i = 0; i < edges.size(); i++) {
    Point a = edges[i].get_a();
    Point b = edges[i].get_b();
    // determine the edge/edge direction
    int diff_x = b.get_x()-a.get_x();
    int diff_y = b.get_y()-a.get_y();
    if (diff_x > 0) diff_x = 1;
    if (diff_x < 0) diff_x = -1;
    if (diff_y > 0) diff_y = 1;
    if (diff_y < 0) diff_y = -1;

    // walk between the islands to draw the bridge
    int x = a.get_x()*2+diff_x;
    int y = a.get_y()*2+diff_y;
    while (x != b.get_x()*2 || y != b.get_y()*2) {
      if (diff_x == 0) {
        // vertical bridge
        assert (diff_y != 0);
        if (board[y][x] == '|') {
          // if we already have a bridge, draw a double vertical bridge
          board[y][x] = '#';
        } else {
          assert (board[y][x] == ' ' || board[y][x]=='-' || board[y][x]=='|' || board[y][x]=='#');
          // otherwise draw a single vertical bridge
          board[y][x] = '|';        
        }
      } else {
        assert (diff_y == 0);
        // horizontal bridge
        if (board[y][x] == '-') {
          // if we already have a brige, draw a double horizontal bridge
          board[y][x] = '=';        
        } else {
          assert (board[y][x] == ' ' || board[y][x] == '|');
          // otherwise draw a single horizontal bridge
          board[y][x] = '-';        
        }
      }
      x+=diff_x;
      y+=diff_y;
    }
  }

  // surround the board with a border of spaces
  char border = ' ';
  std::cout << std::string(board[0].size()+2,border) << std::endl;
  for (int i = board.size()-1; i >= 0; i--) {
    std::cout << border << board[i] << border << std::endl;
  }
  std::cout << std::string(board[0].size()+2,border) << std::endl;
}



//custom sorter function for sorting edges
bool sortEdge(Edge e1, Edge e2){
  std::vector<int> e1Points;
  std::vector<int> e2Points;
  e1Points.push_back(std::max(e1.get_a().get_x(), e1.get_b().get_x()));
  e1Points.push_back(std::min(e1.get_a().get_x(), e1.get_b().get_x()));
  e1Points.push_back(std::max(e1.get_a().get_y(), e1.get_b().get_y()));
  e1Points.push_back(std::min(e1.get_a().get_y(), e1.get_b().get_y()));

  e2Points.push_back(std::max(e2.get_a().get_x(), e2.get_b().get_x()));
  e2Points.push_back(std::min(e2.get_a().get_x(), e2.get_b().get_x()));
  e2Points.push_back(std::max(e2.get_a().get_y(), e2.get_b().get_y()));
  e2Points.push_back(std::min(e2.get_a().get_y(), e2.get_b().get_y()));

  for(int i=0; i<4; i++){
    if(e1Points[i] != e2Points[i]){
      return e1Points[i] < e2Points[i];
    }
  }
  return true;


}








// ====================================================================================
