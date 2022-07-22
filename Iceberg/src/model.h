#include "state.h"
#include "face.h"
#include <cmath>
#include <iostream>
#define X 0
#define Y 1
#define Z 2

class Model{
public:
	Model();
    Model(std::vector<FACE> sides, std::vector<double> params);
	Model(State* x, std::vector<FACE> sides, std::vector<double> params); // инициализирует всё базовыми значениями
	State* state; // хранит текущее состояние
    double density;
    void CheckWater();
	void StepRK4();
	Vector3d GetC();
    std::vector<FACE> UnderWaterSides;
    std::vector<FACE> sides;
    double center;
    double diff;
    void normalize();
    void unnormalize();
    std::vector<double> params;
private:
    Matrix3x3 I_0;
    double mass;
    double volume;
	void f(State* x, State* xdot);
	double h;
    bool isSamePoint(Vector3d a, Vector3d b);
    Vector3d F_ac;
    Vector3d L_dot;
    FACE UnderWaterPart(std::vector<std::vector<Vector3d>> underwater);
};


class Calculator {
    int A;
    int B;
    int C;
    double P1, Pa, Pb, Paa, Pab, Pbb, Paaa, Paab, Pabb, Pbbb;
    double Fa, Fb, Fc, Faa, Fbb, Fcc, Faaa, Fbbb, Fccc, Faab, Fbbc, Fcca;
    double T0, T1[3], T2[3], TP[3];
    void compProjectionIntegrals(FACE *f);
    void compFaceIntegrals(FACE *f);

public:
    void compVolumeIntegrals(Model *p);
    Matrix3x3 getInertiaTensor(Model *p);
    Vector3d getCenterOfMass(Model *p);
    double getMass(Model *p);
    double getVolume(Model *p);
};
