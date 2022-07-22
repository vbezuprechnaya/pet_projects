#ifndef SEIR_Optimizer_H
#define SEIR_Optimizer_H
#include <vector>
#include <random>
#include <cmath>
#include "Model.h"
#include "Matrix.h"


class Optimizer {
public:
    Optimizer(State init_state, std::vector<State> data, std::vector<double> params, double step);
    std::vector<double> predict();
    void CalculateGradient();
    Matrix SystemStepS(Matrix Shat, Matrix Ehat, Matrix Ihat);
    Matrix SystemStepE(Matrix Shat, Matrix Ehat, Matrix Ihat);
    Matrix SystemStepI(Matrix Shat, Matrix Ehat, Matrix Ihat);
private:
    State init_state;
    std::vector<State> data;
    std::vector<double> parameters;
    double step;
    Matrix AInv;
    Matrix AInvReg;
};


#endif //SEIR_Optimizer_H
