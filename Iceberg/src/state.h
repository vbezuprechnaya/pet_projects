#pragma once
#include "MyMatrix.h"

struct State{
    Vector3d c;
    Vector3d p;
    Vector3d w;
    Matrix3x3 R;
    State();
    State(std::vector<double> c_inp, std::vector<double> p_inp, std::vector<double> w_inp, std::vector<std::vector<double>> R_inp);
    friend State operator+(const State &d1, const State &d2);
    friend State operator*(const double &d1, const State &d2);
};
