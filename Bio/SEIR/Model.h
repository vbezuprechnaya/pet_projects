#ifndef SEIR_MODEL_H
#define SEIR_MODEL_H
#define TIME_STEP 0.01
#include "State.h"
#include <vector>
#include <array>
#include <iostream>

class Model {
public:
    Model(State initial_state, std::vector<double>& params);
    void evolve(std::vector<State>& states);
private:
    State Solver(State const& current_state);
    State init_state;
    int N;
    int t;
    double alpha; // Governs the lag between infectious contact and showing symptoms
    double beta;  // Number of people an infective person infects each day
    double gamma; // Probability to recover or die (accounts for two parameters)
    double mu;// death rate
    double epsi;//
};


#endif //SEIR_MODEL_H
