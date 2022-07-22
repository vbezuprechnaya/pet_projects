#include "model.h"
#include "plane.h"
#include <iostream>
#include <algorithm>

Model::Model(){
    density = 916;
    h = 0.001;
    mass = 0;
    F_ac = Vector3d(0, 0, 0);
    L_dot = Vector3d(0, 0, 0);
    I_0 = Matrix3x3();
    state = new State();
}

Model::Model(std::vector<FACE> sides, std::vector<double> params){
    this->params = params;
    density = params[3];
    h = params[0];
    F_ac = Vector3d(0, 0, 0);
    L_dot = Vector3d(0, 0, 0);
    state = new State();
    this->sides = sides;
    Calculator calc;
    calc.compVolumeIntegrals(this);
    mass = calc.getMass(this);
    volume = calc.getVolume(this);
    state->c = calc.getCenterOfMass(this);
    I_0 = calc.getInertiaTensor(this);
    for (int i = 0; i < this->sides.size(); i++){
        for (int j = 0; j < this->sides[i].verts.size(); j++){
            this->sides[i].verts[j] = this->sides[i].verts[j] - state->c;
        }
    }
    this->normalize();
}

Model::Model(State* x0, std::vector<FACE> sides, std::vector<double> params){
    this->params = params;
    density = params[3];
    mass = 0;
    F_ac = Vector3d(0, 0, 0);
    L_dot = Vector3d(0, 0, 0);
	h = params[0];
    state = x0;
    this->sides = sides;
    I_0 = Matrix3x3();
    Calculator calc;
    calc.compVolumeIntegrals(this);
    mass = calc.getMass(this);
    volume = calc.getVolume(this);
    state->c = calc.getCenterOfMass(this);
    I_0 = calc.getInertiaTensor(this);
    for (int i = 0; i < this->sides.size(); i++){
        for (int j = 0; j < this->sides[i].verts.size(); j++){
            this->sides[i].verts[j] = this->sides[i].verts[j] - state->c;
        }
    }
    this->normalize();
}

void Model::f(State* x, State* xdot){
	for (int i=0; i < 3; i++){
        xdot->c[i] = state->p[i] / mass;
    }
    for (int i=0; i < 3; i++){
        xdot->p[i] = this->F_ac[i];
        if (i == 2){
            xdot->p[i] -= this->params[4] * mass;
        }
    }
    CheckWater();
    Vector3d omega = state->R * I_0.inv() * state->R.T() * state->w;

    Matrix3x3 skew({{0, -omega[2], omega[1]},
                         {omega[2], 0, -omega[0]},
                         {-omega[1], omega[0], 0}});
    Matrix3x3 zeros({{0, 0, 0},
                    {0, 0, 0},
                    {0, 0, 0}});
    xdot->R = skew * state->R;
    xdot->w = this->L_dot;

}

void Model::StepRK4(){ // выполняет одну итерацию РК4
    State Xdot;
    State k1, k2, k3, k4, prom;
    f(state, &k1);
    prom = *state + (h / 2) * k1;
    f(&prom, &k2);
    prom = *state + (h / 2) * k2;
    f(&prom, &k3);
    prom = *state + h * k3;
    f(&prom, &k4);
    Xdot = (k1 + 2 * k2 + 2 * k3 + k4);
    *state = *state + h / 6 * Xdot;
    state->w = state->w * this->params[1];
    state->p = state->p * this->params[1];
    CheckWater();
}


Vector3d Model::GetC(){ // возвращает коор. барицентра
	return state->c;
}


void Model::CheckWater(){
    this->F_ac = Vector3d(0, 0, 0);
    this->L_dot = Vector3d(0, 0, 0);
    std::vector<std::vector<Vector3d>> waterlineVertices;
    std::vector<FACE> under_water_sides;
    this->unnormalize();
    this->UnderWaterSides = {};
    //std::cout << this->state->c[0] << " " << this->state->c[1] << " " << this->state->c[2] << "\n";
    waterlineVertices.resize(this->sides.size());//?
    for (int i = 0; i < this->sides.size(); i++){
        waterlineVertices[i].resize(2);
        std::vector<Vector3d> vertices;
        for (int j = 0; j < this->sides[i].verts.size(); j++){
            Vector3d curr = this->state->c + this->state->R * this->sides[i].verts[j];
            Vector3d next = this->state->c + this->state->R * this->sides[i].verts[(j + 1) % this->sides[i].verts.size()];
            if (curr[2] > 0 && next[2] < 0) {
                double k = - curr[2] / (next[2] - curr[2]);
                Vector3d intersec(curr[0] + (next[0] - curr[0]) * k,
                                   curr[1] + (next[1] - curr[1]) * k,
                                   0);
                waterlineVertices[i][0] = intersec;
                vertices.push_back(intersec);
            }
            if (curr[2] <= 0) {
                vertices.emplace_back(curr);
                if (next[2] > 0) {
                    double k = (-curr[2]) / (next[2] - curr[2]);
                    Vector3d intersec(curr[0] + (next[0] - curr[0]) * k,
                                       curr[1] + (next[1] - curr[1]) * k,
                                       0);
                    waterlineVertices[i][1] = intersec;
                    vertices.push_back(intersec);
                }
            }

        }
        if (vertices.size() > 0) {
            FACE newFace;
            newFace.verts = vertices;
            under_water_sides.push_back(newFace);
        }
    }
    if (waterlineVertices.size() > 0) {
        FACE newEdge = UnderWaterPart(waterlineVertices);
        if (newEdge.verts.size() > 2) {
            under_water_sides.push_back(newEdge);
        }
    }
    this->normalize();
    if (under_water_sides.empty()){
        return;
    }
    this->UnderWaterSides = under_water_sides;
    Model* bottom = new Model(under_water_sides, this->params);
    this->F_ac = Vector3d(0, 0, 1) * this->params[2] * this->params[4]  * bottom->volume;
    this->L_dot = Vector3d::cross(bottom->state->c - this->state->c, F_ac);
    std::cout << this->F_ac[2] << " " << bottom->volume << "\n";
    delete bottom;
}

FACE Model::UnderWaterPart(std::vector<std::vector<Vector3d>> underwater){
    FACE newEdge;
    std::vector<Vector3d> vertices;

    for (int i = 0; i < underwater.size(); i++) {
        if (isSamePoint(underwater[i][0], underwater[i][1])) {
            continue;
        } else {
            vertices.push_back(underwater[i][0]);
            vertices.push_back(underwater[i][1]);
            underwater.erase(underwater.begin() + i);
            break;
        }
    }
    if (vertices.empty()) {
        return {};
    }
    while (!isSamePoint(vertices.front(), vertices.back()) && underwater.size() > 0) {//
        for (int i = 0; i < underwater.size(); i++) {
            if (isSamePoint(underwater[i][0], underwater[i][1])) {
                underwater.erase(underwater.begin() + i);
            } else if (isSamePoint(vertices.back(), underwater[i][0])) {
                vertices.push_back(underwater[i][1]);
                underwater.erase(underwater.begin() + i);
                break;
            }
        }
    }
    if (isSamePoint(vertices.front(), vertices.back())) {
        vertices.erase(vertices.end());
    } else {
        vertices.clear();
    }
    newEdge.verts = vertices;
    return newEdge;
}



bool Model::isSamePoint(Vector3d a, Vector3d b){
    return std::abs(a[0] - b[0]) < 0.0001 &&
            std::abs(a[1] - b[1]) < 0.0001 &&
            std::abs(a[2] - b[2]) < 0.0001;
}


void Model::normalize(){
    double min = this->sides[0].verts[0][0];
    double max = this->sides[0].verts[0][0];
    for (int i = 0; i < (int)this->sides.size(); i++){
        for (int j = 0; j < (int)this->sides[i].verts.size(); j++){
            for (int k = 0; k < 3; k++){
                if (min > this->sides[i].verts[j][k]){
                    min = this->sides[i].verts[j][k];
                }
                if (max < this->sides[i].verts[j][k]){
                    max = this->sides[i].verts[j][k];
                }
            }
        }
    }
    this->diff = max - min;
    this->center = (max + min) / 2;
    for (int i = 0; i < (int)this->sides.size(); i++){
        for (int j = 0; j < (int)this->sides[i].verts.size(); j++){
            for (int k = 0; k < 3; k++){
                this->sides[i].verts[j][k] = (this->sides[i].verts[j][k] - this->center) / this->diff;
            }
        }
    }
}

void Model::unnormalize(){
    for (int i = 0; i < (int)this->sides.size(); i++){
        for (int j = 0; j < (int)this->sides[i].verts.size(); j++){
            for (int k = 0; k < 3; k++){
                this->sides[i].verts[j][k] = this->sides[i].verts[j][k] * this->diff + this->center;
            }
        }
    }
}

