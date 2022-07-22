#include "MyMatrix.h"


Matrix3x3::Matrix3x3(){
    this->arr = {{0, 0, 0},
                 {0, 0, 0},
                 {0, 0, 0}};
}

Matrix3x3 Matrix3x3::T(){
    Matrix3x3 c;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            c.arr[i][j] = (*this)(j, i);
        }
    }
    return c;
}

Matrix3x3 Matrix3x3::inv(){
    Matrix3x3 m = *this;
    double det = m(0, 0) * (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) -
                 m(0, 1) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
                 m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0));

    double invdet = 1 / det;

    Matrix3x3 minv; // inverse of matrix m
    minv(0, 0) = (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) * invdet;
    minv(0, 1) = (m(0, 2) * m(2, 1) - m(0, 1) * m(2, 2)) * invdet;
    minv(0, 2) = (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)) * invdet;
    minv(1, 0) = (m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2)) * invdet;
    minv(1, 1) = (m(0, 0) * m(2, 2) - m(0, 2) * m(2, 0)) * invdet;
    minv(1, 2) = (m(1, 0) * m(0, 2) - m(0, 0) * m(1, 2)) * invdet;
    minv(2, 0) = (m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1)) * invdet;
    minv(2, 1) = (m(2, 0) * m(0, 1) - m(0, 0) * m(2, 1)) * invdet;
    minv(2, 2) = (m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1)) * invdet;

    return minv;
}

Matrix3x3::Matrix3x3(std::vector<std::vector<double>> data)
{
    this->arr = data;
}

Matrix3x3 Matrix3x3::operator=(Matrix3x3 m){
    this->arr = m.arr;
    return *this;
}

Vector3d Matrix3x3::operator*(Vector3d v) {
    Vector3d c;
    for (int i = 0; i < 3; i++){
        c.coor[i] = (*this)(i , 0) * v[0] + (*this)(i , 1) * v[1] + (*this)(i , 2) * v[2];
    }
    return c;
}

Matrix3x3 Matrix3x3::operator*(Matrix3x3 m) {
    Matrix3x3 c;
    for (int i = 0; i < 3; i++){
        c.arr[i][0] = (*this)(i , 0) * m(0, 0) + (*this)(i , 1) * m(1, 0) + (*this)(i , 2) * m(2, 0);
        c.arr[i][1] = (*this)(i , 0) * m(0, 1) + (*this)(i , 1) * m(1, 1) + (*this)(i , 2) * m(2, 1);
        c.arr[i][2] = (*this)(i , 0) * m(0, 2) + (*this)(i , 1) * m(1, 2) + (*this)(i , 2) * m(2, 2);
    }
    return c;
}

double& Matrix3x3::operator()(int i, int j){
    return this->arr[i][j];
}

const double& Matrix3x3::operator()(int i, int j) const{
    return this->arr[i][j];
}

Matrix3x3 Matrix3x3::operator+(Matrix3x3 m) {
    Matrix3x3 res;
}

Matrix3x3 Matrix3x3::operator*(double k) {
    Matrix3x3 res;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            res.arr[i][j] = this->arr[i][j] * k;
        }
    }
    return res;
}



