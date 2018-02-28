#include <string>
#include <vector>
#include "utilities.h"

class Polygon{
public:
	Polygon(std::string name_, std::vector<Point> points_): name(name_), points(points_){
		if(points.size()<3){
			throw -1;
		}
		
	}
	std::string getName(){
		return name;
	}
	bool HasAllEqualSides(){
		double dist= DistanceBetween(points[0], points[1]);
		for(int i=0; i<points.size()-1; i++){
			if(!EqualSides(DistanceBetween(points[i], points[i+1]),dist)){
				return false;
			}
		}
		return true;
	}
	bool HasAllEqualAngles(){

		// double a1= Angle(points[0], points[1], points[2]);
		// double a2= Angle(points[1], points[2], points[0]);
		// double a3= Angle(points[2], points[0], points[1]);
		std::vector<double> allAngs;
		for(int i=0; i<points.size(); i++){
			Point p1=points[i];
			Point p2=points[i+1];
			Point p3=points[i+2];
			if(i+1>=points.size()){
				p2= points[(i+1)-points.size()];
			}
			if(i+2>=points.size()){
				p3= points[(i+2)-points.size()];
			}
			allAngs.push_back(Angle(p1, p2, p3));
		}
		double target= allAngs[0];
		for(int i=0; i<allAngs.size(); i++){
			if(!EqualAngles(target, allAngs[i])){
				return false;
			}
		}
		return true;

		// if(EqualAngles(a1,a2) && EqualAngles(a1,a3)){
		// 	return true;
		// }
		// else{
		// 	return false;
		// }
	}
	bool HasARightAngle(){
		
		

		std::vector<double> allAngs;
		for(int i=0; i<points.size(); i++){
			Point p1=points[i];
			Point p2=points[i+1];
			Point p3=points[i+2];
			if(i+1>=points.size()){
				p2= points[(i+1)-points.size()];
			}
			if(i+2>=points.size()){
				p3= points[(i+2)-points.size()];
			}
			allAngs.push_back(Angle(p1, p2, p3));
		}
		double target= allAngs[0];
		for(int i=0; i<allAngs.size(); i++){
			if(RightAngle(allAngs[i])){
				return true;
			}
		}
		return false;
	}
	virtual ~Polygon() {}
protected:
	std::string name;
	std::vector<Point> points;
};

class Quadrilateral: public Polygon{
public:
	Quadrilateral(std::string name_, std::vector<Point> points_): Polygon(name_, points_){
		if(points.size()!=4){
			throw -1;
		}
		
	}
};

class Rectangle: public Quadrilateral{
public:
	Rectangle(std::string name_, std::vector<Point> points_): Quadrilateral(name_, points_){
		
		double angle1= Angle(points[0], points[1], points[2]);
		double angle2= Angle(points[1], points[2], points[3]);
		double angle3= Angle(points[2], points[3], points[0]);
		double angle4= Angle(points[3], points[0], points[1]);
		if(!RightAngle(angle1) || !RightAngle(angle2) || !RightAngle(angle3) || !RightAngle(angle4)){
			throw -1;
		}	
		
	}

};

class Square: public Rectangle{
public:
	Square(std::string name_, std::vector<Point> points_): Rectangle(name_, points_){
		double l1= DistanceBetween(points[0], points[1]);
		double l2= DistanceBetween(points[1], points[2]);
		double l3= DistanceBetween(points[2], points[3]);
		double l4= DistanceBetween(points[3], points[0]);
		if(!EqualSides(l1,l2) || !EqualSides(l1,l3) || !EqualSides(l1,l4)){
			throw -1;
		}
		
	}

};
class Triangle: public Polygon{
public:
	Triangle(std::string name_, std::vector<Point> points_): Polygon(name_, points_){
		if(points.size()!=3){
			throw -1;
		}
		
	}

};

class IsoscelesTriangle: virtual public Triangle{
public:
	IsoscelesTriangle(std::string name_, std::vector<Point> points_): Triangle(name_, points_){
		double l1= DistanceBetween(points[0], points[1]);
		double l2= DistanceBetween(points[1], points[2]);
		double l3= DistanceBetween(points[2], points[0]);
		if(!(EqualSides(l1,l2) || EqualSides(l2,l3) || EqualSides(l3,l1))){
			throw -1;
		}
		
		
	}

};

class EquilateralTriangle: public IsoscelesTriangle{
public:
	EquilateralTriangle(std::string name_, std::vector<Point> points_): IsoscelesTriangle(name_, points_), Triangle(name_, points_){
		double l1= DistanceBetween(points[0], points[1]);
		double l2= DistanceBetween(points[1], points[2]);
		double l3= DistanceBetween(points[2], points[0]);
		if(!(EqualSides(l1,l2) && EqualSides(l1,l3))){
			throw -1;
		}

	}

};

class RightTriangle: virtual public Triangle{
public:
	RightTriangle(std::string name_, std::vector<Point> points_): Triangle(name_, points_){
		double a1= Angle(points[0], points[1], points[2]);
		double a2= Angle(points[1], points[2], points[0]);
		double a3= Angle(points[2], points[0], points[1]);
		if(!(RightAngle(a1) || RightAngle(a2) || RightAngle(a3))){
			throw -1;
		}

	}

};

class IsoscelesRightTriangle: public IsoscelesTriangle, public RightTriangle{
public:
	IsoscelesRightTriangle(std::string name_, std::vector<Point> points_): Triangle(name_,points_), RightTriangle(name_, points_), IsoscelesTriangle(name_, points_){
		// double a1= Angle(points[0], points[1], points[2]);
		// double a2= Angle(points[1], points[2], points[0]);
		// double a3= Angle(points[2], points[0], points[1]);
		// if(!(RightAngle(a1) || RightAngle(a2) || RightAngle(a3))){
		// 	throw -1;
		// }

	}

};
