#include "plane.h"

Plane::Plane(){
	koef = {0, 0, 0};
	point = {0, 0, 0};
}

void Plane::CalcEqu(Vector3d point1, Vector3d point2, Vector3d point3){
	koef[0] = (point2[1] - point2[0]) * (point3[2] - point3[0]) - (point3[1] - point3[0]) * (point2[2] - point2[0]);
	koef[1] = -1 * ((point1[1] - point1[0]) * (point3[2] - point3[0]) - (point3[1] - point3[0]) * (point1[2] - point1[0]));
	koef[2] = (point1[1] - point1[0]) * (point2[2] - point2[0]) - (point2[1] - point2[0]) * (point1[2] - point1[0]);
	point = point1;
}

bool Plane::WherePoint(Vector3d some_point){
	double dist = koef[0] * (point[0] - some_point[0]) + koef[1] * (point[1] - some_point[1]) + koef[2] * (point[2] - some_point[2]);
	return dist > 0; 
}
