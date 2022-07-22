#include "Optimizer.h"

double reg_coef = 0.01;
Optimizer::Optimizer(State init_state, std::vector<State> data, std::vector<double> params, double step) {
    this->init_state = init_state;
    this->data = data;
    this->parameters = params;
    this->step = step;
    Matrix A(this->data.size(), this->data.size());
    double alpha = this->parameters[2], beta = this->parameters[3],
            gamma = this->parameters[4], mu = this->parameters[5], eps = this->parameters[6];

    A(0, 0) = 1;
    A(this->data.size() - 1, this->data.size() - 1) = 1;
    for (int k = 2; k < this->data.size(); k++){
        A(k - 1, k - 2) = 1; A(k - 1, k - 1) = -(2); A(k - 1, k) = 1;
    }
    Matrix L(this->data.size(), this->data.size());
    Matrix U(this->data.size(), this->data.size());
    A.LUdecomp(&L, &U);
    this->AInv = U.T().invertLT().T() * L.invertLT();
    for (int k = 2; k < this->data.size(); k++){
        A(k - 1, k - 2) = 1; A(k - 1, k - 1) = -(2 + reg_coef); A(k - 1, k) = 1;
    }
    Matrix L1(this->data.size(), this->data.size());
    Matrix U1(this->data.size(), this->data.size());
    A.LUdecomp(&L1, &U1);
    this->AInvReg = U1.T().invertLT().T() * L1.invertLT();
    std::cout << "fin";
}

std::vector<double> Optimizer::predict() {
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<> distr(0, 1);
    int cnt = 0;
    while (cnt < 10){
        cnt++;
        this->CalculateGradient();
    }

    return this->parameters;
}

void Optimizer::CalculateGradient(){
    std::vector<State> states{};
    Model Sim(this->init_state, this->parameters);
    Sim.evolve(states);
    Matrix Shat(this->data.size(), 1);
    Matrix Ehat(this->data.size(), 1);
    Matrix Ihat(this->data.size(), 1);
    Shat(0, 0) = this->data[0].S; Ehat(0, 0) = this->data[0].E; Ihat(0, 0) = this->data[0].I;
    for (int i = 1; i < this->data.size() - 1; i++){
        Shat(i, 0) = states[i].S;
        Ehat(i, 0) = states[i].E;
        Ihat(i, 0) = states[i].I;
    }

    Shat(this->data.size() - 1, 0) = this->data[this->data.size() - 1].S;
    Ehat(this->data.size() - 1, 0) = this->data[this->data.size() - 1].E;
    Ihat(this->data.size() - 1, 0) = this->data[this->data.size() - 1].I;

    int cnt = 0;
    while (cnt < 100){
        cnt++;
        for (int i = 1; i < Shat.Size().first; i++){
            Shat(i, 0) = (Shat(i - 1, 0) + Shat(i, 0)) / 2;
            Ehat(i, 0) = (Ehat(i - 1, 0) + Ehat(i, 0)) / 2;
            Ihat(i, 0) = (Ihat(i - 1, 0) + Ihat(i, 0)) / 2;
        }
        Shat = this->SystemStepS(Shat, Ehat, Ihat);
        Ehat = this->SystemStepE(Shat, Ehat, Ihat);
        Ihat = this->SystemStepI(Shat, Ehat, Ihat);
    }

    double a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0, k1 = 0, l = 0, m = 0, n = 0, p = 0;
    double a0 = 0;
    int N = this->parameters[0];
    for (int k = 1; k < states.size(); k++){
        a0 += (states[k].S - states[k - 1].S) * (states[k].S - states[k - 1].S) + (states[k].I - states[k - 1].I) * (states[k].I - states[k - 1].I) + (states[k].E - states[k - 1].E) * (states[k].E - states[k - 1].E);
        a += (2 * TIME_STEP * (states[k].I - states[k - 1].I) * Ehat(k, 0) -
              2 * TIME_STEP * (states[k].E - states[k - 1].E) * Ehat(k, 0)); // eps
        b += -2 * TIME_STEP * (states[k].I - states[k - 1].I) * Ihat(k, 0); // gamma, alpha
        c += (-2 * TIME_STEP * (states[k].I - states[k - 1].I) * Ihat(k, 0) -
              2 * TIME_STEP * (states[k].E - states[k - 1].E) * Ehat(k, 0) +
              2 * TIME_STEP * (states[k].S - states[k - 1].S) * (N - Shat(k, 0))); // mu
        d += (2 * (states[k].E - states[k - 1].E) * TIME_STEP * Shat(k, 0) * Ihat(k, 0) / N -
              2 * (states[k].S - states[k - 1].S) * TIME_STEP * Shat(k, 0) * Ihat(k, 0) / N); // beta
        e += 2 * TIME_STEP * TIME_STEP * Ehat(k, 0) * Ehat(k, 0); // eps * eps
        f += TIME_STEP * TIME_STEP * Ihat(k, 0) * Ihat(k, 0); // alpha * alpha, gamma * gamma
        h += TIME_STEP * TIME_STEP * (Ehat(k, 0) * Ehat(k, 0) + Ihat(k, 0) * Ihat(k, 0) + (N - Shat(k, 0)) * (N - Shat(k, 0))); // mu * mu
        g += 2 * TIME_STEP * TIME_STEP * Shat(k, 0) * Shat(k, 0) * Ihat(k, 0) * Ihat(k, 0) / (N * N); // beta * beta
        k1 += -2 * TIME_STEP * TIME_STEP * Ehat(k, 0) * Ihat(k, 0); // eps * gamma, eps * alpha
        n += -2 * TIME_STEP * TIME_STEP * Shat(k, 0) * Ihat(k, 0) * Ehat(k, 0) / N; // beta * eps
        l += 2 * TIME_STEP * TIME_STEP * Ihat(k, 0) * Ihat(k, 0); // gamma * mu, gamma * alpha, mu * alpha
        m += -2 * TIME_STEP * TIME_STEP * (Ehat(k, 0) * Ihat(k, 0) - Ehat(k, 0) * Ehat(k, 0)); // mu * eps
        p += (-2 * TIME_STEP * TIME_STEP * Shat(k, 0) * Ihat(k, 0) * Ehat(k, 0) / N -
              2 * TIME_STEP * TIME_STEP * (N - Shat(k, 0)) * Shat(k, 0) * Ihat(k, 0) / N); // beta * mu
    }

    this->parameters[2] = (-4*a*g*h*k1 + 2*a*g*l*m + a*k1*p*p - a*l*n*p + 8*b*e*g*h - 2*b*e*p*p - 2*b*g*m*m - 2*b*h*n*n + 2*b*m*n*p - 4*c*e*g*l + 2*c*g*k1*m - c*k1*n*p + c*l*n*n + 2*d*e*l*p + 2*d*h*k1*n - d*k1*m*p - d*l*m*n)/(-16*e*f*g*h + 4*e*f*p*p - 8*e*g*h*l + 8*e*g*l*l + 2*e*l*p*p + 4*f*g*m*m + 4*f*h*n*n - 4*f*m*n*p + 8*g*h*k1*k1 - 8*g*k1*l*m + 2*g*l*m*m + 2*h*l*n*n - 2*k1*k1*p*p + 4*k1*l*n*p - 2*l*l*n*n - 2*l*m*n*p);
    this->parameters[3] = (-4*a*f*h*n + 2*a*f*m*p - 2*a*h*l*n - 2*a*k1*l*p + 2*a*l*l*n + a*l*m*p + 4*b*e*l*p + 4*b*h*k1*n - 2*b*k1*m*p - 2*b*l*m*n - 4*c*e*f*p - 2*c*e*l*p + 2*c*f*m*n + 2*c*k1*k1*p - 2*c*k1*l*n + c*l*m*n + 8*d*e*f*h + 4*d*e*h*l - 4*d*e*l*l - 2*d*f*m*m - 4*d*h*k1*k1 + 4*d*k1*l*m - d*l*m*m)/(-16*e*f*g*h + 4*e*f*p*p - 8*e*g*h*l + 8*e*g*l*l + 2*e*l*p*p + 4*f*g*m*m + 4*f*h*n*n - 4*f*m*n*p + 8*g*h*k1*k1 - 8*g*k1*l*m + 2*g*l*m*m + 2*h*l*n*n - 2*k1*k1*p*p + 4*k1*l*n*p - 2*l*l*n*n - 2*l*m*n*p);
    this->parameters[4] = (-4*a*g*h*k1 + 2*a*g*l*m + a*k1*p*p - a*l*n*p + 8*b*e*g*h - 2*b*e*p*p - 2*b*g*m*m - 2*b*h*n*n + 2*b*m*n*p - 4*c*e*g*l + 2*c*g*k1*m - c*k1*n*p + c*l*n*n + 2*d*e*l*p + 2*d*h*k1*n - d*k1*m*p - d*l*m*n)/(-16*e*f*g*h + 4*e*f*p*p - 8*e*g*h*l + 8*e*g*l*l + 2*e*l*p*p + 4*f*g*m*m + 4*f*h*n*n - 4*f*m*n*p + 8*g*h*k1*k1 - 8*g*k1*l*m + 2*g*l*m*m + 2*h*l*n*n - 2*k1*k1*p*p + 4*k1*l*n*p - 2*l*l*n*n - 2*l*m*n*p);
    this->parameters[5] = (-4*a*f*g*m + 2*a*f*n*p + 4*a*g*k1*l - 2*a*g*l*m + a*l*n*p - 8*b*e*g*l + 4*b*g*k1*m - 2*b*k1*n*p + 2*b*l*n*n + 8*c*e*f*g + 4*c*e*g*l - 2*c*f*n*n - 4*c*g*k1*k1 - c*l*n*n - 4*d*e*f*p - 2*d*e*l*p + 2*d*f*m*n + 2*d*k1*k1*p - 2*d*k1*l*n + d*l*m*n)/(-16*e*f*g*h + 4*e*f*p*p - 8*e*g*h*l + 8*e*g*l*l + 2*e*l*p*p + 4*f*g*m*m + 4*f*h*n*n - 4*f*m*n*p + 8*g*h*k1*k1 - 8*g*k1*l*m + 2*g*l*m*m + 2*h*l*n*n - 2*k1*k1*p*p + 4*k1*l*n*p - 2*l*l*n*n - 2*l*m*n*p);
    this->parameters[6] = (8*a*f*g*h - 2*a*f*p*p + 4*a*g*h*l - 4*a*g*l*l - a*l*p*p - 8*b*g*h*k1 + 4*b*g*l*m + 2*b*k1*p*p - 2*b*l*n*p - 4*c*f*g*m + 2*c*f*n*p + 4*c*g*k1*l - 2*c*g*l*m + c*l*n*p - 4*d*f*h*n + 2*d*f*m*p - 2*d*h*l*n - 2*d*k1*l*p + 2*d*l*l*n + d*l*m*p)/(-16*e*f*g*h + 4*e*f*p*p - 8*e*g*h*l + 8*e*g*l*l + 2*e*l*p*p + 4*f*g*m*m + 4*f*h*n*n - 4*f*m*n*p + 8*g*h*k1*k1 - 8*g*k1*l*m + 2*g*l*m*m + 2*h*l*n*n - 2*k1*k1*p*p + 4*k1*l*n*p - 2*l*l*n*n - 2*l*m*n*p);

//    double phi =  a0 + a * eps + b * (gamma + alpha) + c * mu + d * beta + e * eps * eps + f * (gamma * gamma + alpha * alpha) +
//                  g * beta * beta + h * mu * mu + k1 * (eps * gamma + eps * alpha) + m * eps * mu + n * eps * beta +
//                  l * (gamma * alpha + gamma * mu + alpha * mu) + p * mu + beta;
//
//    double phi_1 = 0;
//    for (int k = 1; k < states.size(); k++){
//        phi_1 += std::pow(states[k].S - states[k - 1].S + TIME_STEP * (mu * (N - Shat(k, 0) - beta * Shat(k, 0) * Ihat(k, 0) / N)), 2);
//        phi_1 += std::pow(states[k].E - states[k - 1].E + TIME_STEP * (beta * Shat(k, 0) * Ihat(k, 0) / N - (mu + eps) * Ehat(k, 0)), 2);
//        phi_1 += std::pow(states[k].I - states[k - 1].I + TIME_STEP * (eps * Ehat(k, 0)  - (gamma + mu + alpha) * Ihat(k, 0)), 2);
//    }
//
//    alpha = this->parameters[2], beta = this->parameters[3],
//    gamma = this->parameters[4], mu = this->parameters[5], eps = this->parameters[6];
//
//    double phi_2 = a0 + a * eps + b * (gamma + alpha) + c * mu + d * beta + e * eps * eps + f * (gamma * gamma + alpha * alpha) +
//                   g * beta * beta + h * mu * mu + k1 * (eps * gamma + eps * alpha) + m * eps * mu + n * eps * beta +
//                   l * (gamma * alpha + gamma * mu + alpha * mu) + p * mu + beta;
//
//    double phi_3 = 0;
//    double comp_1 = 0, comp_2 = 0, comp_3 = 0;
//    for (int k = 1; k < states.size(); k++){
//        comp_1 += std::pow(states[k].S - states[k - 1].S + TIME_STEP * (mu * (N - Shat(k, 0)) - beta * Shat(k, 0) * Ihat(k, 0) / N), 2);
//        comp_2 += std::pow(states[k].E - states[k - 1].E + TIME_STEP * (beta * Shat(k, 0) * Ihat(k, 0) / N - (mu + eps) * Ehat(k, 0)), 2);
//        comp_3 += std::pow(states[k].I - states[k - 1].I + TIME_STEP * (eps * Ehat(k, 0)  - (gamma + mu + alpha) * Ihat(k, 0)), 2);
//    }
//    phi_3 = comp_1 + comp_2 + comp_3;
}

Matrix Optimizer::SystemStepS(Matrix Shat, Matrix Ehat, Matrix Ihat){
    Matrix b(this->data.size(), 1);
    double N = this->data[0].S + this->data[0].E + this->data[0].I + this->data[0].R;
    double alpha = this->parameters[2], beta = this->parameters[3],
            gamma = this->parameters[4], mu = this->parameters[5], eps = this->parameters[6];

    b(0, 0) = this->data[0].S;
    //N = this->data[this->data.size() - 1].S + this->data[this->data.size() - 1].E + this->data[this->data.size() - 1].I + this->data[this->data.size() - 1].R;
    b(this->data.size() - 1, 0) = this->data[this->data.size() - 1].S;

    for (int k = 2; k < this->data.size() - 1; k++){
        //N = this->data[k].S + this->data[k].E + this->data[k].I + this->data[k].R;
        b(k - 1, 0) = TIME_STEP * (mu * (Shat(k - 1, 0) - Shat(k, 0)) -
                beta * (Shat(k, 0) * Ihat(k, 0) - Shat(k - 1, 0) * Ihat(k - 1, 0)) / N);
    }

    Shat = AInv * b;
    return Shat;
}


Matrix Optimizer::SystemStepE(Matrix Shat, Matrix Ehat, Matrix Ihat){
    Matrix b(this->data.size(), 1);
    double N = this->data[0].S + this->data[0].E + this->data[0].I + this->data[0].R;
    double alpha = this->parameters[2], beta = this->parameters[3],
            gamma = this->parameters[4], mu = this->parameters[5], eps = this->parameters[6];


    b(0, 0) = this->data[0].E;
    //N = this->data[this->data.size() - 1].S + this->data[this->data.size() - 1].E + this->data[this->data.size() - 1].I + this->data[this->data.size() - 1].R;
    b(this->data.size() - 1, 0) = this->data[this->data.size() - 1].E;

    for (int k = 2; k < this->data.size() - 1; k++){
        //N = this->data[k].S + this->data[k].E + this->data[k].I + this->data[k].R;
        b(k - 1, 0) = TIME_STEP * (beta * (Shat(k, 0) * Ihat(k, 0) / N - Shat(k - 1, 0) * Ihat(k - 1, 0) / N) -
                                   (mu + eps) * (Ehat(k, 0) - Ehat(k - 1, 0)));
    }

    Ehat = AInv * b;
    return Ehat;
}


Matrix Optimizer::SystemStepI(Matrix Shat, Matrix Ehat, Matrix Ihat){
    Matrix b(this->data.size(), 1);
    double alpha = this->parameters[2], beta = this->parameters[3],
            gamma = this->parameters[4], mu = this->parameters[5], eps = this->parameters[6];

    b(0, 0) = this->data[0].I;

    b(this->data.size() - 1, 0) = this->data[this->data.size() - 1].I;

    for (int k = 2; k < this->data.size() - 1; k++){
        b(k - 1, 0) = TIME_STEP * (eps * (Ehat(k, 0) - Ehat(k - 1, 0)) -
                                   (gamma + mu + alpha) * (Ihat(k, 0) - Ihat(k - 1, 0))) - reg_coef * this->data[k - 1].I;
    }

    Ihat = AInvReg * b;
    return Ihat;
}


