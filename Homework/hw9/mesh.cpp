#include <vector>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <cstdio>

#include "mesh.h"
#include "image.h"


// to ensure that the edge lengths are unique, add a bit of random
// noise to the coordinates
#define RANDOM_JITTER 0.2

double random_float() { return std::rand() / double(RAND_MAX); }

// =======================================================================================
// =======================================================================================

Mesh::Mesh(const std::string &filename, int num_rows, int num_cols, 
           const std::string &which_, const std::string &method_, 
           bool preserve_area_, bool debug_) : 
  which(which_), method(method_), 
  preserve_area(preserve_area_), debug(debug_), 
  next_random_edge(NULL) {

  // we use a fixed seed for the random number generator (for repeatability)
  std::srand(42);
  
  // load the image
  Image image;
  image.Load(filename);

  // scale the image to a 1000x1000 bounding box
  width  = double(MAXIMUM_SVG)*image.Width() /double(std::max(image.Height(),image.Width()));
  height = double(MAXIMUM_SVG)*image.Height()/double(std::max(image.Height(),image.Width()));
  
  // calculate the spacing necessary for a perfect grid with the requested resolution
  double dx = width  / double(num_cols);
  double dy = height / double(num_rows);

  // first, create the coordinates
  std::vector<Vertex*> points;
  // NOTE: The coordinate system for our images starts with (0,0) in
  // the upper left corner.  Positive x goes to the right: (0,1000) is
  // the upper right corner.  Positive y goes down: (1000,0) is the
  // lower left corner.
  for (int j = 0; j < (num_rows+1); j++) {
    for (int i = 0; i < (num_cols+1); i++) {
      double rand_dx = 2 * random_float() - 1.0;
      double rand_dy = 2 * random_float() - 1.0;
      if (i == 0 || i == num_cols) rand_dx = 0;
      if (j == 0 || j == num_rows) rand_dy = 0;
      // to ensure that the edge lengths are unique, add a bit of random
      // noise to the coordinates
      double x = BORDER + i*dx + rand_dx*RANDOM_JITTER*dx;
      double y = BORDER + j*dy + rand_dy*RANDOM_JITTER*dy;
      int i_ = i * image.Width()/ (num_cols+1);
      int j_ = j * image.Height()/ (num_rows+1);
      Color c = image.GetPixel(i_,image.Height()-j_-1);
      float r = c.r;
      float g = c.g;
      float b = c.b;
      Vertex* v = new Vertex(x,y, r,g,b);
      AddVertex(v);
      points.push_back(v);
    }
  }
  // then, create the triangles
  for (int j = 0; j < (num_rows); j++) {
    for (int i = 0; i < (num_cols); i++) {
      Vertex *a = points[i  + j   *(num_cols+1)];
      Vertex *b = points[i+1+ j   *(num_cols+1)];
      Vertex *c = points[i+1+(j+1)*(num_cols+1)];
      Vertex *d = points[i  +(j+1)*(num_cols+1)];
      // NOTE: The order of the vertices for a triangle is important.
      // The vertices should be in a consistent clockwise order so it
      // is "right side up".
      if (random_float() > 0.5) {
        AddTriangle(a,b,c); 
        AddTriangle(a,c,d); 
      } else {
        AddTriangle(a,b,d);
        AddTriangle(b,c,d);
      }
    }
  }
  
}


Mesh::~Mesh() {
  // delete the dynamically allocated memory for the edges, triangles,
  // and vertices
  for (edges_map::iterator itr = edges.begin();
       itr != edges.end(); itr++) {
    delete itr->second;
  }
  for (triangles_set::iterator itr = triangles.begin();
       itr != triangles.end(); itr++) {
    delete *itr;
  }
  for (vertices_set::iterator itr = vertices.begin();
       itr != vertices.end(); itr++) {
    delete *itr;
  }
}


double Mesh::getArea() const {
  double area=0;
  // simply loop over all triangles
  for (triangles_set::const_iterator itr = triangles.begin(); itr != triangles.end(); itr++) {
    area += (*itr)->getArea();
  }
  return area;
}

// =======================================================================================

// Determine if this collapse preserves the right-side-up property and
// does not change the overall area of the mesh
bool Mesh::isLegalCollapse(Edge *e) const {
  assert (e != NULL);
  // only check the legality of an edge collapse if we must preserve
  // the overall mesh area
  if (!preserve_area) return true;

  // sum the area of all triangles touching either vertex of the edge
  // before and after the collapse
  double before_sum = 0;
  double after_sum = 0;
  
  Vertex *a = e->getV1();
  Vertex *b = e->getV2();
  
  // the geometric coordinate after the edge is collapsed
  Point avg = Average(a->getPoint(),b->getPoint());
  
  // loop over all of the triangles that contain either or both
  // vertices.  
  //
  // triangles with both vertices will be deleted in the collapse, so
  // don't count their area after the collapse.
  //
  // if one of the triangles flips "upside down" as a result of the
  // collapse, disallow this modification.
  for (std::set<Triangle*>::const_iterator itr = a->getTriangles().begin();
       itr != a->getTriangles().end(); itr++) {
    Triangle *t = (*itr);
    assert (t->HasVertex(a));
    before_sum += t->getArea();
    if (!(t->HasVertex(b))) {
      after_sum += t->getAreaAfterReplacement(a,avg);
      if (!t->RightSideUpAfterReplacement(a,avg)) return false;
    }
  }
  for (std::set<Triangle*>::const_iterator itr = b->getTriangles().begin();
       itr != b->getTriangles().end(); itr++) {
    Triangle *t = (*itr);
    assert (t->HasVertex(b));
    if (!(t->HasVertex(a))) {
      before_sum += t->getArea();
      after_sum += t->getAreaAfterReplacement(b,avg);
      if (!t->RightSideUpAfterReplacement(b,avg)) return false;
    }
  }

  // if the area changes significantly, do not allow this collapse
  if (fabs(before_sum-after_sum)/(before_sum+after_sum) > 0.001) {
    return false;
  }

  // looks good!
  return true; 
}


// Modify the mesh to collapse / squeeze the two vertices of this edge
// to a single point that is the average of the two points.  Remove
// the one or two triangles that border this edge and change the
// triangles to use the averaged vertex.
bool Mesh::Collapse() {

  Edge *e = FindEdge();
  if (e == NULL) {
    // return false if nothing changed
    return false; 
  }

  //assigns the verticies for the edge you are planning to collapse
  Vertex *a = e->getV1();
  Vertex *b = e->getV2();
  assert (a->getID() < b->getID());
  //sets a to the point in between a and b
  a->Average(b);

  //copy of triangles to iterate over
  triangles_set copy_tri = triangles;
  //
  std::set<Edge*> edgesToRemove;  
  
  for (triangles_set::iterator itr = copy_tri.begin(); itr != copy_tri.end(); itr++) {
    Triangle *t = *itr;
    //vector that holds the two verticies not touching b
    std::vector<Vertex*> notTouching;
    //if triangle is touching both a and b then delete them
    if (t->HasVertex(a) && t->HasVertex(b)) {
      RemoveTriangle(t);
    }
    //if it only has vertex b, then move the points that were previously touching b to a 
    else if(t->HasVertex(b)){
      //removes the triangle from b
      b->RemoveTriangle(t);
      for(int i = 0; i < 3; i++){
        if(t->pts[i]==b){
          
          t->pts[i]=a;
          //adds the triangle to a
          a->AddTriangle(t);
          
        }
        else{
          //if it isn't one of the vertexes touching b, then add it to not touching
          notTouching.push_back(t->pts[i]);
        }
      }
      assert(notTouching.size()==2);
      //add both of the edges from the points not touching b to the a vertex
      AddEdge(notTouching[0], a);
      AddEdge(notTouching[1], a);


    }
    


  }
  
  //delete any edges that have b in it after it is disassosiated from everything
  //adds all edges touching b to a set that will be deleted
  for(std::set<Edge*>::iterator itr = b->getEdges().begin(); itr!=b->getEdges().end(); itr++){
    
    edgesToRemove.insert(*itr);
  }
  //delete all edges that were touching b
  for(std::set<Edge*>::iterator itr = edgesToRemove.begin(); itr!=edgesToRemove.end(); itr++){
    RemoveEdge((*itr)->getV1(), (*itr)->getV2());
  }
  
  //once completely disassosiated with all triangles and edges b can be deleted
  RemoveVertex(b);
  
  //recalculated the edges for all edges touching a
  ReCalculateEdges(a->getEdges());
  //checks validity of heap
  next_random_edge= NULL;
  return true;

}



// Collapse one edge at a time until the # of triangles is <= the
// target count.
void Mesh::Simplify(int target_count) {
  while (numTriangles() > target_count) {
    // we might get stuck before reaching the target count
    bool success = Collapse();
    if (!success) break;
  }
  Check();
}


// =======================================================================================
// PRIVATE MODIFIERS FOR MESH MANIPULATION

void Mesh::AddVertex(Vertex *a) {
  vertices.insert(a);
}

void Mesh::RemoveVertex(Vertex *a) {
  // make sure no neighbors depend on this vertex
  assert (a->getTriangles().size() == 0);
  assert (a->getEdges().size() == 0);
  int ret = vertices.erase(a);
  assert (ret == 1);
  // cleanup the dynamically allocated memory
  delete a;
}


void Mesh::AddEdge(Vertex *a_, Vertex *b_) {
  Vertex *a;
  Vertex *b;
  // for consistency, reorder the vertices so that the first vertex
  // has the lower id
  if (a_->getID() < b_->getID()) {
    a = a_; b = b_;
  } else {
    a = b_; b = a_;
  }
  // search in the map to see if the edge already exists
  std::pair<Vertex*,Vertex*> e(a,b);
  edges_map::iterator itr = edges.find(e);
  if (itr == edges.end()) {
    Edge *ep = new Edge(a,b,this);
    edges[e] = ep;
    // add the edge to the list of neighbors maintained by each vertex
    a->AddEdge(ep);
    b->AddEdge(ep);
    //add the edge to the priority queue when you add it to the mesh
    edges_pq.push(ep);
    // ASSIGNMENT: additional code needed for priority queue
    //


  }
  assert (edges.find(e) != edges.end());
}

void Mesh::RemoveEdge(Vertex *a_, Vertex *b_) {
  Vertex *a;
  Vertex *b;
  // for consistency, reorder the vertices so that the first vertex
  // has the lower id
  if (a_->getID() < b_->getID()) {
    a = a_; b = b_;
  } else {
    a = b_; b = a_;
  }
  // search in the map to find the edge
  std::pair<Vertex*,Vertex*> e(a,b);
  edges_map::iterator itr = edges.find(e);
  if (itr != edges.end()) {

    //
    // ASSIGNMENT: additional code needed for priority queue
    //

    // remove the edge from the list of neighbors maintained by each vertex
    a->RemoveEdge(itr->second);
    b->RemoveEdge(itr->second);
    //remove the edge from the priority queue when you remove it from the mesh
    edges_pq.remove(itr->second);
    // clean up the dynamically allocated memory
    delete itr->second;
    edges.erase(itr);
  }
  assert (edges.find(e) == edges.end());
}


void Mesh::AddTriangle(Vertex *a, Vertex *b, Vertex *c) {
  Triangle *t = new Triangle(a,b,c,this);
  triangles.insert(t);
  // add this triangle to the list of neighbors maintained by each vertex
  a->AddTriangle(t);
  b->AddTriangle(t);
  c->AddTriangle(t);
  // add the 3 edges to the mesh (if they don't already exist)
  AddEdge(a,b);
  AddEdge(b,c);
  AddEdge(c,a);
  
  // because the addition of this triangle may impact the legality
  // (either making a previously legal collapse illegal or vice
  // versa... ), we must recalculate the value for each edge that
  // touches any of the vertices of this triangle
  std::set<Edge*> collected_edges;
  CollectEdgesWithVertex(a,collected_edges);
  CollectEdgesWithVertex(b,collected_edges);
  CollectEdgesWithVertex(c,collected_edges);
  ReCalculateEdges(collected_edges);
}

void Mesh::RemoveTriangle(Triangle *t) {
  // remove the triangle from the list of neighbors maintained by each vertex
  t->getVertex(0)->RemoveTriangle(t);
  t->getVertex(1)->RemoveTriangle(t);
  t->getVertex(2)->RemoveTriangle(t);
  // clean up the dynamically allocated memory
  triangles.erase(t);
  delete t;
}

// =======================================================================================
// =======================================================================================

void Mesh::CollectEdgesWithVertex(Vertex *a,std::set<Edge*>& r_e) const {
  const std::set<Edge*>& edges = a->getEdges();
  for (std::set<Edge*>::const_iterator itr = edges.begin(); itr != edges.end(); itr++) {
    r_e.insert(*itr);
  }
}


// We have collected edges that might change collapse legality or
// priority value.  Loop over them and update the data.  (The position
// in the priority queue may also need to change!)
void Mesh::ReCalculateEdges(const std::set<Edge*>& r_e)  {
  for (std::set<Edge*>::iterator itr = r_e.begin(); itr != r_e.end(); itr++) {
    (*itr)->ReCalculateValue();

    //update the positions of the edges that you pass in in the priority queue
    edges_pq.update_position(*itr);
    //
    // ASSIGNMENT: additional code needed for priority queue
    //

  }
}

// =======================================================================================
// =======================================================================================

Edge* Mesh::FindEdge() const {
  Edge* answer = NULL;

  // ============================================
  // choose a random edge to collapse
  if (which == "random") {
    // NOTE: We store the next random edge so that we can visualize it
    // before collapse. 
    if (next_random_edge == NULL) {
      // collect all of the legal edges (in a vector for easy random selection)
      std::vector<Edge*> legal_edges;
      for (edges_map::const_iterator itr = edges.begin(); itr != edges.end(); itr++) {
        Edge* e = itr->second;
        if (e->isLegal()) {
          legal_edges.push_back(e);
        }
      }
      // pick one
      if (legal_edges.size() > 0) {
        int rand = std::rand() % legal_edges.size();
        assert (rand >= 0 && rand < (int)legal_edges.size());
        ((Mesh*)(this))->next_random_edge = legal_edges[rand];
      }
    }
    answer = next_random_edge;
  } 

  // ============================================
  // these two different methods are handled in the edge
  else if (which == "shortest" || which == "color") {
    // perform a simple linear search over all edges to find the best edge
    if (method == "linear") {
      for (edges_map::const_iterator itr = edges.begin(); itr != edges.end(); itr++) {
        Edge* e = itr->second;
        if (e->isLegal() && (answer == NULL || e->getLength() < answer->getLength())) {
          answer = e;
        }
      }
    } 
    // perform a priority queue to find the best edge
    else if (method == "priority_queue") {

      //set the edge to be deleted to the top value in the priority queue
      answer= edges_pq.top();
      //
      // ASSIGNMENT: finish the implementation for the priority queue
      //

    }
    else {
      std::cerr << "ERROR!  Unknown method to find best edge " << method << std::endl;
    }
  }

  // ============================================
  else {
    std::cerr << "ERROR!  Unknown edge choice " << which << std::endl;
  }
  return answer;
}



// =======================================================================================
// =======================================================================================

// a helper function for debugging
void Mesh::Check() const {
  if (!debug) return;

  std::cout << "Mesh Check... ";
  fflush(stdout);

  // loop over all of the triangles
  for (triangles_set::const_iterator itr = triangles.begin(); 
       itr != triangles.end(); itr++) {
    Triangle *t = *itr;
    for (int i = 0; i < 3; i++) {
      Vertex *v = t->getVertex(i);
      // make sure the triangle is in the list for each of its 3 vertices
      std::set<Triangle*>::iterator itr3 = v->getTriangles().find(t);
      assert (itr3 != v->getTriangles().end());
      // make sure the 3 edges are in the edge map
      Vertex *v2 = t->getVertex((i+1)%3);
      std::pair<Vertex*,Vertex*> e;
      if (v->getID() < v2->getID()) {
        e = std::pair<Vertex*,Vertex*>(v,v2);
      } else {
        e = std::pair<Vertex*,Vertex*>(v2,v);
      }
      edges_map::const_iterator itr = edges.find(e);
      assert (itr != edges.end());
    }
  }

  // loop over all of the vertices
  for (vertices_set::const_iterator itr = vertices.begin(); 
       itr != vertices.end(); itr++) {
    Vertex *v = *itr;
    // make sure all triangles listed at this vertex actually have this vertex
    for (std::set<Triangle*>::const_iterator itr = v->getTriangles().begin();
         itr != v->getTriangles().end(); itr++) {
      Triangle *t = *itr;
      assert (t->HasVertex(v));
    }
  }

  // loop over all of the edges
  for (std::map<std::pair<Vertex*,Vertex*>,Edge*>::const_iterator itr = edges.begin(); 
       itr != edges.end(); itr++) {
    Vertex *v1 = itr->first.first;
    Vertex *v2 = itr->first.second;
    assert (v1->getID() < v2->getID());
    // loop over all of the triangles for one of the vertices, and
    // make sure there is at least one triangle that uses this edge
    int count = 0;
    for (std::set<Triangle*>::const_iterator itr2 = v1->getTriangles().begin(); 
         itr2 != v1->getTriangles().end(); itr2++) {
      Triangle *t = *itr2;
      assert (t->HasVertex(v1));
      if (t->HasVertex(v2)) {
        count++;
      }
    }
    // each edge should belong to 2 triangles, except the border,
    // where each edge belongs to 1 triangle

    
    assert (count == 1 || count == 2);
  }

  std::cout << "completed" << std::endl;
}

// =======================================================================================

std::ostream& operator<<(std::ostream &ostr, const Mesh& m) {
  ostr << std::setw(6) << m.numVertices() << " vertices, "
       << std::setw(6) << m.numEdges() << " edges, "
       << std::setw(6) << m.numTriangles() << " triangles, "
       << "area = " << std::setw(10) << std::fixed << std::setprecision(2) 
       << m.getArea() << std::endl;
  return ostr;
}

// =======================================================================================

