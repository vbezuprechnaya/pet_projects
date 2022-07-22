#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "model.h"
#include "plane.h"
#include "MyMatrix.h"
#include <chrono>
#include <thread>
#include <cmath>
#include <cstdlib>
#include <iostream>


class Visual{
public:
	// тут Callbackи - это такая хитрость чтобы подружить OpenGL и классы
    static void renderCallback();
    static void TimerCallback(int value);
    static void changeCallback(int w, int h);
    void ShowPolyhedron();
    void ShowUnderWater();
    void ShowWater();
    static Visual* currentInstance;
    Visual();
    Visual(State& state, std::vector<Vector3d> points, std::vector<double> params);
    void changeSize(int w, int h);
    void renderScene(void);
    void init(int argc, char **argv);
private:
	Model m;
};
