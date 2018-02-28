#include <cstdlib>

#include "edge.h"
#include "mesh.h"

// =================================================================
// CONSTRUCTOR

Edge::Edge(Vertex *a, Vertex *b, Mesh *m) {
  assert (a->getID() != b->getID());
  // Order the vertices so the smallest vertex id # is first.
  if (a->getID() < b->getID()) {
    v1 = a;
    v2 = b;
  } else {
    v1 = b;
    v2 = a;
  }
  mesh = m;
  length = -1;
  legal = false;
  ReCalculateValue();
}

// =================================================================
// ACCESSOR

double Edge::getPriorityValue() const {
  if (mesh->getWhichCollapse() == "shortest") {
    if (!legal) {
      return 10000;
    }
    return length;
  } else if (mesh->getWhichCollapse() == "color") {
    

    //
    // ASSIGNMENT:  EXTRA CREDIT
    // replace with something that better preserves the image
    //
    return length;


  } else {
    std::cerr << "ERROR: Unknown choice of next edge to collapse" << std::endl;
    exit(1);
  }
}


// When one of the endpoints of this edge, or a neighboring edge is
// changed, the legality of edge collapse and/or the priority value of
// this edge may change.
void Edge::ReCalculateValue() {
  length = DistanceBetween(v1->getPoint(),v2->getPoint());
  legal = mesh->isLegalCollapse(this);
}


// A helper function for error checking
bool Edge::CheckValue() const {
  double length2 = DistanceBetween(v1->getPoint(),v2->getPoint());
  bool legal2 = mesh->isLegalCollapse((Edge*)this);
  bool error = false;
  if (legal != legal2) {
    std::cout << "LEGALITY ERROR! " << *this << " " << legal << " should be " << legal2 << std::endl;
    error = true;
  }
  if (fabs(length-length2) > 0.0001) {
    std::cout << "LENGTH ERROR!   " << *this << " " << length << " should be " << length2 << std::endl;
    error = true;
  }
  return error;
}


Vertex* Edge::getMidPoint() const{
  double newX= (getV1()->x()+getV2()->x())/2;
  double newY= (getV1()->y()+getV2()->y())/2;
  //probably should get the average of the previous 2 vertex rgb instead of jus 0 lol
  Vertex* newVertex= new Vertex(newX, newY, (getV1()->r()+getV2()->r())/2, 
                                            (getV1()->b()+getV2()->b())/2,
                                            (getV1()->g()+getV2()->g())/2);

  return newVertex;
}




// =================================================================
// stream printing functions for Edge and Edge*

std::ostream& operator<<(std::ostream &ostr, const Edge& e) {
  ostr << "EDGE " << e.getV1()->getID() << " " << e.getV2()->getID();
  if (e.isLegal()) {
    ostr << "  len="  << DistanceBetween(e.getV1()->getPoint(),e.getV2()->getPoint());
  }
  return ostr;
}

std::ostream& operator<<(std::ostream &ostr, Edge* e) {
  ostr << *e; 
  return ostr;
}

// =================================================================
