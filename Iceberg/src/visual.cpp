#include "visual.h"

void Visual::changeSize(int w, int h){
    // предотвращение деления на ноль
    if (h == 0)
        h = 1;
    float ratio =  w * 1.0 / h;
    // используем матрицу проекции
    glMatrixMode(GL_PROJECTION);
    // обнуляем матрицу
    glLoadIdentity();
    // установить параметры вьюпорта
    glViewport(0, 0, w, h);
    // установить корректную перспективу
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    // вернуться к матрице проекции
    glMatrixMode(GL_MODELVIEW);
}

void Visual::ShowPolyhedron(){
    glBegin(GL_TRIANGLES);

    //front triangle
    glColor3f(125,125,0);
    for (int i = 0; i < (int)currentInstance->m.sides.size(); i++){
        for (int j = 0; j < (int)currentInstance->m.sides[i].verts.size(); j++){
            double x = currentInstance->m.sides[i].verts[j][0] * currentInstance->m.diff + currentInstance->m.center;
            double y = currentInstance->m.sides[i].verts[j][1] * currentInstance->m.diff + currentInstance->m.center;
            double z = currentInstance->m.sides[i].verts[j][2] * currentInstance->m.diff + currentInstance->m.center;
            Vector3d coor(x, y, z);
            coor = currentInstance->m.state->R * coor;
            glColor3f(125,0,0);
            glVertex3f(currentInstance->m.state->c[0] + coor[0],
                       currentInstance->m.state->c[1] + coor[1],
                       currentInstance->m.state->c[2] + coor[2]);
            glColor3f(0,0,125);
        }
    }
    glEnd();
    glPointSize(10);
    glBegin(GL_POINTS);
    glColor3f(0,125,0);
    glVertex3f(currentInstance->m.state->c[0], currentInstance->m.state->c[1], currentInstance->m.state->c[2]);
    glEnd();
    //exit(0);
}

void Visual::ShowUnderWater(){
    glBegin(GL_LINES);
    glColor3f(255,255,255);
    for (int i = 0; i < currentInstance->m.UnderWaterSides.size(); i++){
        for (int j = 0; j < currentInstance->m.UnderWaterSides[i].verts.size(); j++){
            glVertex3f(currentInstance->m.UnderWaterSides[i].verts[j][0],
                       currentInstance->m.UnderWaterSides[i].verts[j][1],
                       currentInstance->m.UnderWaterSides[i].verts[j][2]);
            glVertex3f(currentInstance->m.UnderWaterSides[i].verts[(j + 1) % currentInstance->m.UnderWaterSides[i].verts.size()][0],
                       currentInstance->m.UnderWaterSides[i].verts[(j + 1) % currentInstance->m.UnderWaterSides[i].verts.size()][1],
                       currentInstance->m.UnderWaterSides[i].verts[(j + 1) % currentInstance->m.UnderWaterSides[i].verts.size()][2]);
        }
    }
    glEnd();
    if (currentInstance->m.UnderWaterSides.size() > 0){
        Vector3d Center = currentInstance->m.GetC();
        glPointSize(10);
        glBegin(GL_POINTS);
        glColor3f(255,203,219);
        glVertex3f(Center[0], Center[1], Center[2]);
        glEnd();
    }
}

void Visual::ShowWater(){
    glColor4f(0.0f, .0f, 255.0f, 0.5);
    glBegin(GL_QUADS);
    glVertex3f(10.0f, 10.0f, 0.0f);
    glVertex3f(10.0f, -10.0f, 0.0f);
    glVertex3f(-10.0f, -10.0f, 0.0f);
    glVertex3f(-10.0f, 10.0f, 0.0f);
    glEnd();
}

void Visual::renderScene(void){
    // очистка буфера цвета и глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // обнуление трансформации
    glLoadIdentity();
    // установка камеры
    gluLookAt( 0.0f, 25.0f, 9.0f,
               0.0f, 0.0f, 0.0f,
               0.0f, 0.0f, 1.0f);

 	currentInstance->m.StepRK4();
    ShowPolyhedron();
    ShowUnderWater();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ShowWater();
    glDisable(GL_BLEND);
    glutSwapBuffers();
}


void Visual::init(int argc, char **argv){
    // Инициализация GLUT и создание окна
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(1000,1000);
    glutCreateWindow("Iceberg");
    glEnable(GL_DEPTH_TEST);
    // регистрация
    glutDisplayFunc(Visual::renderCallback);
    glutReshapeFunc(Visual::changeCallback);
    glutTimerFunc(1, Visual::TimerCallback, 0);
    // основной цикл
    glutMainLoop();
}

void Visual::renderCallback(){
    currentInstance->renderScene();
}

void Visual::TimerCallback(int value){
    glutPostRedisplay();  // Redraw windows
    glutTimerFunc(1, TimerCallback, 0); // Setup next timer
}

void Visual::changeCallback(int w, int h){
    currentInstance->changeSize(w, h);
}

Visual::Visual(){
	currentInstance = this;
	currentInstance->m = Model();
}

Visual::Visual(State& state, std::vector<Vector3d> points, std::vector<double> params){
	currentInstance = this;
    std::vector<FACE> edges;
    FACE new_side({points[2], points[3], points[1]});
    edges.push_back(new_side);
    FACE new_sid({points[3], points[2], points[0]});
    edges.push_back(new_sid);
    FACE newsid({points[3], points[0], points[1]});
    edges.push_back(newsid);
    FACE newsd({points[1], points[0], points[2]});
    edges.push_back(newsd);
	currentInstance->m = Model(&state, edges, params);
}
