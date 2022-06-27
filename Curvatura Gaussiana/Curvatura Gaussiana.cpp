#include <iostream>
#include "Function.h"
#include <iostream>
using namespace std;

Function F;

float gamma = 90;
float phi = 45;
float theta = 135;

void config()
{
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-3, 3, -3, 3, -3, 3);
    glRotatef(gamma, 0, 0, 1);
    glRotatef(phi, 0, 1, 0);
    glRotatef(theta, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
}

void display()
{
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    F.display_surface();
    glutSwapBuffers();
}

int main(int argc, char**argv)
{
    float dx, dy, dz;
    float cgmax = 0, cgmin = 0;
    float cmax = 0, cmin = 0;

    for (dx = -1; dx < 1; dx += 0.2) {
        for (dy = -1; dy < 1; dy += 0.2) {
            for (dz = -1; dz < 1; dz += 0.2) {
                float xyz[3] = { dx, dy, dz };
                float grad[3];
                float h[9];
                float hstar[9];


                F.normal(xyz, grad);
                F.hess(xyz, h);
                F.hs(h, hstar);

                float cg = F.curvatura_gaussiana(grad, h, hstar);
                float cm = F.curvatura_media(grad, h);

                if (cg > cgmax)
                    cgmax = cg;
                if (cg < cgmin)
                    cgmin = cg;
                if (cm > cmax)
                    cmax = cm;
                if (cm < cmin)
                    cmin = cm;
            }
        }
    }
    //cout << cgmax << ", " << cgmin << endl;
    //cout << cmax << ", " << cmin << endl;
    cout << "Curvatura Gaussiana: digite 0" << endl;
    cout << "Curvatura Media: digite 1" << endl;
    cin >> F.GaussOrMedian;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Curvatura Gaussiana");
    config();
    glutDisplayFunc(display);
    glutMainLoop();
}