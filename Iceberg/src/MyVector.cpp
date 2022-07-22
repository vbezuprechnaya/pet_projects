#include "MyVector.h"

Vector3d Vector3d::normalize()
{
    Vector3d res;
    res = (*this) *  (1 / this->abs());
    return res;
}
double Vector3d::abs()
{
    double res;
    for (int i = 0; i < 3; i++){
        res += this->coor[i] * this->coor[i];
    }
    return sqrt(res);
}


Vector3d Vector3d::operator+(const Vector3d rv)
{
    return Vector3d(this->coor[0] + rv.coor[0], this->coor[1] + rv.coor[1], this->coor[2] + rv.coor[2]);
}

Vector3d Vector3d::operator-(const Vector3d rv)
{
    return Vector3d(this->coor[0] - rv.coor[0], this->coor[1] - rv.coor[1], this->coor[2] - rv.coor[2]);
}


Vector3d Vector3d::operator*(const double rv)
{
    return Vector3d(this->coor[0] * rv, this->coor[1] * rv, this->coor[2] * rv);
}

Vector3d Vector3d::cross(const Vector3d l, const Vector3d r)
{
    Vector3d res(l.coor[1] * r.coor[2] - l.coor[2] * r.coor[1],
                 l.coor[2] * r.coor[0] - l.coor[0] * r.coor[2],
                 l.coor[0] * r.coor[1] - l.coor[1] * r.coor[0]);
    return res;
}

double& Vector3d::operator[](int index)
{
    return this->coor[index];
}

const double& Vector3d::operator[](int index) const{
    return this->coor[index];
}

double Vector3d::dot(const Vector3d l, const Vector3d r)
{
    double res = 0;
    for (int i =0; i < 3; i++){
        res += l.coor[i] * r.coor[i];
    }
    return res;
}
