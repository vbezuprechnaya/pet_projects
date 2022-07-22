#include "state.h"

State::State(){
    c = {0, 0, 0};
    p = {0, 0, 0};
    w =  {0, 0, 0};
    R = Matrix3x3({{1, 0, 0},
                   {0, 1, 0},
                   {0, 0, 1}});
}

State::State(std::vector<double> c_inp, std::vector<double> p_inp, std::vector<double> w_inp, std::vector<std::vector<double>> R_inp){
    c = c_inp;
    p = p_inp;
    w = w_inp;
    R = R_inp;
}

State operator+(const State &d1, const State &d2)
{
    State res;
    for (int i = 0; i < 3; i++){
        res.c[i] = d1.c[i] + d2.c[i];
        res.p[i] = d1.p[i] + d2.p[i];
        res.w[i] = d1.w[i] + d2.w[i];
        for (int j = 0; j < 3; j++){
            res.R(i, j) = d1.R(i, j) + d2.R(i, j);
        }
    }
    return res;
}

State operator*(const double &d1, const State &d2){
    State res;
    for (int i = 0; i < 3; i++){
        res.c[i] = d1 * d2.c[i];
        res.p[i] = d1 * d2.p[i];
        res.w[i] = d1 * d2.w[i];
        for (int j = 0; j < 3; j++){
            res.R(i, j) = d1 * d2.R(i, j);
        }
    }
    return res;
}
