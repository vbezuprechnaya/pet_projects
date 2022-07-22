#include <iostream>
#include "sir.hpp"
#include "Matrix.h"

double mu = 10e-2;

Matrix MatrixInv(Matrix A){
    int n = A.Size().first;
    Matrix L(n, n);
    Matrix U(n, n);
    for (int i = 0; i < n; i++){
        L(i, i) = 1;
    }
    double sum = 0;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (i <= j){
                for (int k = 0; k < i; k++){
                    sum += L(i, k) * U(k, j);
                }
                U(i, j) = A(i, j) - sum;
                sum = 0;
            } else{
                for (int k = 0; k < j; k++){
                    sum += L(i, k) * U(k, j);
                }
                L(i, j) = (A(i, j) - sum) / U(j, j);
                sum = 0;
            }
        }
    }

    Matrix Uinv = U.T().invertLT().T();
    Matrix Linv = L.invertLT();



    return Uinv * Linv;
}

Matrix SystemStepS(Matrix Shat, Matrix Ihat, Matrix Rhat, Matrix AInv, std::vector<seir::State> states, double alpha, double beta){
    Matrix b(states.size(), 1);

    b(0, 0) = states[0].S;
    b(states.size() - 1, 0) = states[states.size() - 1].S;

    for (int k = 2; k < states.size() - 1; k++){
        b(k - 1, 0) = alpha * (Shat(k - 1, 0) * Ihat(k - 1, 0) - Shat(k, 0) * Ihat(k, 0));
        b(k - 1, 0) = seir::TIME_STEP * b(k - 1, 0);
    }

    Shat = AInv * b;
    return Shat;
}
Matrix SystemStepI(Matrix Shat, Matrix Ihat, Matrix Rhat, Matrix AInv, std::vector<seir::State> states, double alpha, double beta){
    Matrix b(states.size(), 1);

    b(0, 0) = states[0].I;
    b(states.size() - 1, 0) = states[states.size() - 1].I;

    for (int k = 2; k < states.size() - 1; k++){
        b(k - 1, 0) = alpha * (Shat(k, 0) * Ihat(k, 0) - Shat(k - 1, 0) * Ihat(k - 1, 0)) +
                beta * (Ihat(k - 1, 0) - Ihat(k, 0));
        b(k - 1, 0) = seir::TIME_STEP * b(k - 1, 0) - mu * states[k - 1].I;
    }

    Ihat = AInv * b;
    return Ihat;
}


int main() {
    seir::State init{762, 1, 0};
    int time = 5;
    seir::SEIR Model(763, time, init, 0.00218, 0.4485);
    std::vector<seir::State> states = {};
    Model.evolve(states);

    Matrix Shat(states.size(), 1);
    Matrix Shatprev(states.size(), 1);
    Matrix Ihat(states.size(), 1);
    Matrix Rhat(states.size(), 1);

    Matrix A(states.size(), states.size());

    A(0, 0) = 1;
    int size = states.size();
    A(states.size()- 1, states.size() - 1) = 1;
    for (int k = 2; k < states.size(); k++){
        A(k - 1, k - 2) = 1; A(k - 1, k - 1) = -(2 + mu); A(k - 1, k) = 1;
    }

    Matrix AInv = MatrixInv(A);
    double s = states[states.size() - 1].S + states[states.size() - 1].I + states[states.size() - 1].R;
    double appr_alpha = 0.1, appr_beta = 0.3;
    int cnt_big = 0;
    while (cnt_big < 20){
        cnt_big++;
        seir::SEIR Model_appr(763, time, init, appr_alpha, appr_beta);
        std::vector<seir::State> states_appr = {};
        Model_appr.evolve(states_appr);

        Shat(0, 0) = states[0].S; Rhat(0, 0) = states[0].R; Ihat(0, 0) = states[0].I;
        for (int i = 1; i < states.size() - 1; i++){
            Shat(i, 0) = states[i].S;
            Rhat(i, 0) = states[i].R;
            Ihat(i, 0) = states[i].I;
        }

        Shat(states.size() - 1, 0) = states[states.size() - 1].S;
        Ihat(states.size() - 1, 0) = states[states.size() - 1].I;
        Rhat(states.size() - 1, 0) = states[states.size() - 1].R;


        int cnt = 0;
        Shatprev = Shat;
        while (cnt < 100){
            cnt++;
            for (int i = 1; i < Shat.Size().first; i++){
                Shat(i, 0) = (Shat(i - 1, 0) + Shat(i, 0)) / 2;
                Ihat(i, 0) = (Ihat(i - 1, 0) + Ihat(i, 0)) / 2;
            }

            Shat = SystemStepS(Shat, Ihat, Rhat, AInv, states, appr_alpha, appr_beta);
            Ihat = SystemStepI(Shat, Ihat, Rhat, AInv, states, appr_alpha, appr_beta);

            Shatprev = Shat;
        }

        double a00 = 0, a10 = 0, a01 = 0, a20 = 0, a11 = 0, a02 = 0;
        for (int i = 1; i < states.size(); i++){
            a00 += (states[i].I - states[i - 1].I) * (states[i].I - states[i - 1].I);
            a00 += (states[i].S - states[i - 1].S) * (states[i].S - states[i - 1].S);
            a10 += -2 * Ihat(i, 0) * Shat(i, 0) * (states[i].I - states[i - 1].I - states[i].S + states[i - 1].S);
            a10 = a10 * seir::TIME_STEP;
            a01 += 2 * Ihat(i, 0) * (states[i].I - states[i - 1].I);
            a01 = a01 * seir::TIME_STEP;
            a20 += 2 * Ihat(i, 0) * Ihat(i, 0) * Shat(i, 0) * Shat(i, 0);
            a20 = a20 * seir::TIME_STEP * seir::TIME_STEP;
            a11 += -2 * Ihat(i, 0) * Ihat(i, 0) * Shat(i, 0);
            a11 = a11 * seir::TIME_STEP * seir::TIME_STEP;
            a02 += Ihat(i, 0) * Ihat(i, 0);
            a02 = a02 * seir::TIME_STEP * seir::TIME_STEP;
        }

        appr_alpha = - (2 * a02 * a10 - a01 * a11) / (-a11 * a11 + 4 * a02 * a20);
        appr_beta = - (a10 * a11 - 2 * a01 * a20) / (a11 * a11 - 4 * a02 * a20);

    }
    std::cout<< "appr_alpha = " << appr_alpha <<  std::endl;
    std::cout<< "aappr_beta = " << appr_beta <<  std::endl;
    return 0;
}
