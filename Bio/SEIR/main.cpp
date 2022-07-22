#include <iostream>
#include <fstream>
#include "Model.h"
#include "Optimizer.h"

int main() {
//    Matrix Deriv(5, 1);
//    Matrix Psi(4, 1);
//    for (int i = 0; i < 4; i++){
//        Psi(i, 0) = -3;
//    }
//    Matrix Jacobian({{1, -4, 6, -9, 22},
//                     {0, 24, -1, 97, 32},
//                     {-4, 7, 8, 3, 11},
//                     {-4, -6, -1, -23, 9}});
//    Deriv = Deriv + (Psi.T() * Jacobian).T();



    State init_state{100000 - 2001, 2000, 1, 0}; // восприимчивые, подвергшиеся воздействию, инфицированные, выздоровевшие
    std::vector<double> params = {100000, 10, 0.006, 0.75, 0.125, 0.00008, 1.0 / 3.0}; // popul, time, alpha, beta, gamma, mu, epsi
    Model Simulation(init_state, params);
    /* α: средний уровень летальности, вызванной вирусом.
     * β: Вероятность передачи болезни при контакте
     * μ: Естественная смертность на душу населения.
     * ϵ: скорость перехода от контакта к инфекции (обратная величина — инкубационный период).
     * γ: скорость выздоровления заразных лиц */

    std::vector<State> states{};

    Simulation.evolve(states);

    std::vector<double> init_params = {100000, 10, 0.006, 0.75, 0.125, 0.00008, 1.0 / 3.0}; // popul, time, alpha, beta, gamma, mu, epsi

    Optimizer Solver(init_state, states, init_params, 0.001);

    std::vector<double> some = Solver.predict();




    std::ofstream result;
    result.open("../data.txt");

    for (int i = 0; i < states.size(); i++){
        result << states[i].S << " " << states[i].E << " " << states[i].I << " " << states[i].R << "\n";
    }

    std::cout << states.size();
    result.close();

    return 0;
}
