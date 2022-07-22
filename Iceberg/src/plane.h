#include <vector>
#include "MyMatrix.h"

class Plane{
public:
	Plane();
	void CalcEqu(Vector3d point1, Vector3d point2, Vector3d point3);
	bool WherePoint(Vector3d some_point);
private:
	std::vector<double> koef;
    Vector3d point;
};
