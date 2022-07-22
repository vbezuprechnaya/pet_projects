#include "Model.h"

Model::Model(State initial_state,  std::vector<double>& params) {
    this->N = params[0];
    this->t = params[1];
    this->init_state = initial_state;
    this->alpha = params[2];
    this->beta = params[3];
    this->gamma = params[4];
    this->mu = params[5];
    this->epsi = params[6];
}

State Model::Solver(State const& current_state)
{
    State newState;
    State updatedState;

    // 1st order term
    // k1 = f(S,E,I,R)
    std::array<double, 4> k1{};
    k1[0] = TIME_STEP * current_state.dS_dt(mu, beta, N);
    k1[1] = TIME_STEP * current_state.dE_dt(beta, epsi, mu, N);
    k1[2] = TIME_STEP * current_state.dI_dt(alpha, epsi, mu,gamma);
    k1[3] = TIME_STEP * current_state.dR_dt(gamma, mu);

    // Update the state by half a TIME_STEP
    updatedState.S = current_state.S + k1[0] / 2.0;
    updatedState.E = current_state.E + k1[1] / 2.0;
    updatedState.I = current_state.I + k1[2] / 2.0;
    updatedState.R = current_state.R + k1[3] / 2.0;

    // 2nd order term
    // k2 = f(S+h/2*k1 ,E+h/2*k1 ,I+h/2*k1 ,R+h/2*k1)
    std::array<double, 4> k2{};
    k2[0] = TIME_STEP * updatedState.dS_dt(mu, beta, N);
    k2[1] = TIME_STEP * updatedState.dE_dt(beta, epsi, mu, N);
    k2[2] = TIME_STEP * updatedState.dI_dt(alpha, epsi, mu,gamma);
    k2[3] = TIME_STEP * updatedState.dR_dt(gamma, mu);

    // Update the state by half a TIME_STEP
    updatedState.S = current_state.S + k2[0] / 2.0;
    updatedState.E = current_state.E + k2[1] / 2.0;
    updatedState.I = current_state.I + k2[2] / 2.0;
    updatedState.R = current_state.R + k2[3] / 2.0;

    // 3rd order term
    // k3 = f(S+h/2*k ,E+h/2*k2 ,I+h/2*k2 ,R+h/2*k2)
    std::array<double, 4> k3{};
    k3[0] = TIME_STEP * updatedState.dS_dt(mu, beta, N);
    k3[1] = TIME_STEP * updatedState.dE_dt(beta, epsi, mu, N);
    k3[2] = TIME_STEP * updatedState.dI_dt(alpha, epsi, mu, gamma);
    k3[3] = TIME_STEP * updatedState.dR_dt(gamma, mu);

    // Update the state by the whole TIME_STEP
    updatedState.S = current_state.S + k3[0];
    updatedState.E = current_state.E + k3[1];
    updatedState.I = current_state.I + k3[2];
    updatedState.R = current_state.R + k3[3];

    // 4th order term
    // k4 = f(S+h ,E+h ,I+h ,R+h)
    std::array<double, 4> k4{};
    k4[0] = TIME_STEP * updatedState.dS_dt(mu,beta, N);
    k4[1] = TIME_STEP * updatedState.dE_dt(beta, epsi, mu, N);
    k4[2] = TIME_STEP * updatedState.dI_dt(alpha, epsi, mu, gamma);
    k4[3] = TIME_STEP * updatedState.dR_dt(gamma, mu);

    // Calculating the values of S,E,I,R for the new state
    // y_{n+1}= y_n + 1/6(k1+2k2+2k3+k4)
    newState.S = current_state.S + (k1[0] + 2.0 * k2[0] + 2.0 * k3[0] + k4[0]) / 6.0;
    newState.E = current_state.E + (k1[1] + 2.0 * k2[1] + 2.0 * k3[1] + k4[1]) / 6.0;
    newState.I = current_state.I + (k1[2] + 2.0 * k2[2] + 2.0 * k3[2] + k4[2]) / 6.0;
    newState.R = current_state.R + (k1[3] + 2.0 * k2[3] + 2.0 * k3[3] + k4[3]) / 6.0;

    // Handle case when one variable is negative
    if (newState.S < 0) newState.S = 0;
    if (newState.E < 0) newState.E = 0;
    if (newState.I < 0) newState.I = 0;
    if (newState.R < 0) newState.R = 0;

    return newState;
}

void Model::evolve(std::vector<State>& states)
{

    states.reserve(t);

    State current_state = this->init_state;
    State new_state;

    states.push_back(current_state);
    double time = 0;
    while (time < t){
        new_state = Solver(current_state);
        states.push_back(new_state);
        current_state = new_state;
        time += TIME_STEP;
    }

//    for (int time = 0; time < t; time += TIME_STEP)
//    {
//        new_state = Solver(current_state);
//        states.push_back(new_state);
//        current_state = new_state;
//    }
}