#pragma once
#include "MyVector.h"

struct FACE{
    FACE(){};
    FACE(std::vector<Vector3d> p) : verts(p) {};
    std::vector<Vector3d> verts;
    Vector3d GetNormal(){
        Vector3d v1 = verts[2] - verts[1], v2 = verts[0] - verts[1];
        Vector3d some = Vector3d::cross(v1, v2);
        return some;
    }
};
