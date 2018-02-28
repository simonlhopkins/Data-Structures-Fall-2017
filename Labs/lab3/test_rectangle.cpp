#include <iostream>
#include "Rectangle.h"

int main(){

	Rectangle r1= Rectangle(Point2D(0,0), Point2D(5,5));
	Point2D p1= Point2D(3,44);
	Point2D p2= Point2D(3,3);
	Point2D p3= Point2D(3,4);
	Point2D p4= Point2D(5,5);
	Point2D p5= Point2D(5,5.1);
	Point2D p6= Point2D(-44,44);
	r1.add_point(p1);
	r1.add_point(p2);
	r1.add_point(p3);
	r1.add_point(p4);
	r1.add_point(p5);
	r1.add_point(p6);
	std::cout<<"This should be false: "<<r1.is_point_within(p1)<<std::endl;
	std::cout<<"This should be true: "<<r1.is_point_within(p2)<<std::endl;
	std::cout<<"This should be false: "<<r1.is_point_within(p3)<<std::endl;
	print_rectangle(r1);

	return 0;
}