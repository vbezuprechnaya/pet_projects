#pragma once
#include <cmath>
#include <vector>

class Vector3d{
public:
    std::vector<double> coor;
    Vector3d() : coor({0, 0, 0}) {}
    Vector3d(std::vector<double> inp) : coor(inp) {}
    Vector3d(double x, double y, double z) : coor({x, y, z}) {}
    Vector3d operator+(const Vector3d rv);
    Vector3d operator-(const Vector3d rv);
    Vector3d operator*(const double rv);
    static Vector3d cross(const Vector3d l, const Vector3d r);
    static double dot(const Vector3d l, const Vector3d r);
    const double& operator[](int index) const;
    double& operator[](int index);
    Vector3d normalize();
    double abs();
};
