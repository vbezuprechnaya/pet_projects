#pragma once
#include <vector>
#include "MyVector.h"

class Matrix3x3{
public:
	std::vector<std::vector<double>> arr;
    Matrix3x3();
	Matrix3x3(std::vector<std::vector<double>> data);
	Matrix3x3 T();
    Matrix3x3 inv();
	Vector3d operator*(const Vector3d v);
	Matrix3x3 operator*(const Matrix3x3 m);
	Matrix3x3 operator+(const Matrix3x3 m);
	Matrix3x3 operator*(const double k);
    double& operator()(int i, int j);
    const double& operator()(int i, int j) const;
	Matrix3x3 operator=(Matrix3x3 m);
};
