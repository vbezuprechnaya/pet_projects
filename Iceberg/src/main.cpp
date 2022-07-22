#include "visual.h"

Visual* Visual::currentInstance = NULL;
int main(int argc, char **argv){
	std::vector<double> c = {0, 0, 0};
	std::vector<double> p = {0, 0, 0}; // начальный импульс
	std::vector<double> w = {1, 0, 0}; // начальная угловая скорость
	std::vector<std::vector<double>> R = {  // начальная матрица поворота
            {1, 0, 0},
            {0, 1, 0},
            {0, 0, 1}
    };
	State state(c, p, w, R);
    std::vector<Vector3d> points = {Vector3d(0, 0, -2),
                                    Vector3d(0, 3, -2),
                                    Vector3d(0, 0, 1),
                                    Vector3d(3, 0, -2)};
    std::vector<double> params = {0.001, 0.9999, 1030, 916, 9.81}; // шаг интегрирования, коэфф для потерь, плотность воды, плотность айсберга
    Visual* Some = new Visual(state, points, params);
    Some->init(argc, argv);
    return 0;
}