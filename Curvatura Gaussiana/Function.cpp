#include "Function.h"
//#define LINE 0

void Function::display_surface()
{
	float dx = (xmax - xmin) / n;
	float dy = (ymax - ymin) / n;
	float dz = (zmax - zmin) / n;
	float x, y, z;
	float v[8][3];
	x = xmin;
	for (int i = 0; i < n; i++) {
		y = ymin;
		for (int j = 0; j < n; j++) {
			z = zmin;
			for (int k = 0; k < n; k++) {
				v[0][0] = x; v[1][0] = x + dx;
				v[0][1] = y; v[1][1] = y;
				v[0][2] = z; v[1][2] = z;

				v[2][0] = x;	v[3][0] = x + dx;
				v[2][1] = y + dy; v[3][1] = y + dy;
				v[2][2] = z;	v[3][2] = z;

				v[4][0] = x;	v[5][0] = x + dx;
				v[4][1] = y;	v[5][1] = y;
				v[4][2] = z + dz; v[5][2] = z + dz;

				v[6][0] = x;	  v[7][0] = x + dx;
				v[6][1] = y + dy; v[7][1] = y + dy;
				v[6][2] = z + dz;	  v[7][2] = z + dz;
				tetraedro(v[0], v[1], v[3], v[7]);
				tetraedro(v[0], v[1], v[5], v[7]);
				tetraedro(v[0], v[2], v[3], v[7]);
				tetraedro(v[0], v[2], v[6], v[7]);
				tetraedro(v[0], v[4], v[5], v[7]);
				tetraedro(v[0], v[4], v[6], v[7]);
				z += dz;
			}
			y += dy;
		}
		x += dx;
	}
}
void Function::tetraedro(float* v0, float* v1, float* v2, float* v3)
{
	float w[4];
	float p[4][3];
	float nv[3];
	float h[9];
	float hstar[9];
	float curvaturaGaussiana[4];
	float xyzG[4][3];
	int ind = 0;
	//glDisable(GL_LIGHTING);
	glColor3f(1, 0, 1);
	/*glBegin(GL_LINE_LOOP);
	glVertex3fv(v0);
	glVertex3fv(v1);
	glVertex3fv(v2);
	glEnd();
	glBegin(GL_LINES);
	glVertex3fv(v0);
	glVertex3fv(v3);
	glVertex3fv(v1);
	glVertex3fv(v3);
	glVertex3fv(v2);
	glVertex3fv(v3);
	glEnd();*/
	//glEnable(GL_LIGHTING);
	w[0] = f(v0[0], v0[1], v0[2]);
	w[1] = f(v1[0], v1[1], v1[2]);
	w[2] = f(v2[0], v2[1], v2[2]);
	w[3] = f(v3[0], v3[1], v3[2]);

	float t;

	if (w[0] * w[1] < 0) {
		t = -w[0] / (w[1] - w[0]);
		for (int i = 0; i < 3; i++)
			p[ind][i] = v0[i] + t * (v1[i] - v0[i]);
		ind++;
	}

	if (w[1] * w[3] < 0) {
		t = -w[1] / (w[3] - w[1]);
		for (int i = 0; i < 3; i++)
			p[ind][i] = v1[i] + t * (v3[i] - v1[i]);
		ind++;
	}

	if (w[0] * w[3] < 0) {
		t = -w[0] / (w[3] - w[0]);
		for (int i = 0; i < 3; i++)
			p[ind][i] = v0[i] + t * (v3[i] - v0[i]);
		ind++;
	}

	if (w[2] * w[3] < 0) {
		t = -w[2] / (w[3] - w[2]);
		for (int i = 0; i < 3; i++)
			p[ind][i] = v2[i] + t * (v3[i] - v2[i]);
		ind++;
	}

	if (w[0] * w[2] < 0) {
		t = -w[0] / (w[2] - w[0]);
		for (int i = 0; i < 3; i++)
			p[ind][i] = v0[i] + t * (v2[i] - v0[i]);
		ind++;
	}

	if (w[1] * w[2] < 0) {
		t = -w[1] / (w[2] - w[1]);
		for (int i = 0; i < 3; i++)
			p[ind][i] = v1[i] + t * (v2[i] - v1[i]);
		ind++;
	}
	if (ind == 3) {
		
#ifdef LINE
		glBegin(GL_LINE_LOOP);
		glVertex3fv(p[0]);
		glVertex3fv(p[1]);
		glVertex3fv(p[2]);
		glEnd();
#endif
#ifndef LINE
		glBegin(GL_TRIANGLES);
		normal(p[0], nv);
		hess(p[0], h);
		hs(h, hstar);
		curvaturaGaussiana[0] = curvatura_gaussiana(nv, h, hstar);
		glColor3f(-(curvaturaGaussiana[0] - 1) / 2, (curvaturaGaussiana[0] + 1) / 2, 0);
		//glNormal3fv(nv);
		glVertex3fv(p[0]);

		normal(p[1], nv);
		hess(p[1], h);
		hs(h, hstar);
		curvaturaGaussiana[1] = curvatura_gaussiana(nv, h, hstar);
		glColor3f(-(curvaturaGaussiana[1] - 1) / 2, (curvaturaGaussiana[1] + 1) / 2, 0);
		//glNormal3fv(nv);
		glVertex3fv(p[1]);

		normal(p[2], nv);
		hess(p[2], h);
		hs(h, hstar);
		curvaturaGaussiana[2] = curvatura_gaussiana(nv, h, hstar);
		glColor3f(-(curvaturaGaussiana[2] - 1) / 2, (curvaturaGaussiana[2] + 1) / 2, 0);
		//glNormal3fv(nv);
		glVertex3fv(p[2]);
		glEnd();

		/*bool temgauss[3] = {false,false,false};
		if (curvaturaGaussiana[0] * curvaturaGaussiana[1] < 0) {
			xyzG[0][0] = (p[0][0] + p[1][0]) / 2;
			xyzG[0][1] = (p[0][1] + p[1][1]) / 2;
			xyzG[0][2] = (p[0][2] + p[1][2]) / 2;
			temgauss[0] = true;
		}
		if (curvaturaGaussiana[0] * curvaturaGaussiana[2] < 0) {
			xyzG[1][0] = (p[0][0] + p[2][0]) / 2;
			xyzG[1][1] = (p[0][1] + p[2][1]) / 2;
			xyzG[1][2] = (p[0][2] + p[2][2]) / 2;
			temgauss[1] = true;
		}
		if (curvaturaGaussiana[1] * curvaturaGaussiana[2] < 0) {
			xyzG[2][0] = (p[1][0] + p[2][0]) / 2;
			xyzG[2][1] = (p[1][1] + p[2][1]) / 2;
			xyzG[2][2] = (p[1][2] + p[2][2]) / 2;
			temgauss[2] = true;
		}
		if (temgauss[0] && temgauss[1]) {
			glColor3f(0, 0, 1);
			glBegin(GL_LINE);
			glVertex3fv(xyzG[0]);
			glVertex3fv(xyzG[1]);
			glEnd();
		}
		if (temgauss[2] && temgauss[1]) {
			glColor3f(0, 0, 1);
			glBegin(GL_LINE);
			glVertex3fv(xyzG[2]);
			glVertex3fv(xyzG[1]);
			glEnd();
		}
		if (temgauss[0] && temgauss[2]) {
			glColor3f(0, 0, 1);
			glBegin(GL_LINE);
			glVertex3fv(xyzG[0]);
			glVertex3fv(xyzG[2]);
			glEnd();
		}*/
#endif

	}
	if (ind == 4) {
		glColor3f(1, 0, 0); // MUDAR AQUI
#ifdef LINE
		glBegin(GL_LINE_LOOP);
		glVertex3fv(p[0]);
		glVertex3fv(p[1]);
		glVertex3fv(p[2]);
		glVertex3fv(p[3]);
		glEnd();
#endif
#ifndef LINE
		glBegin(GL_QUADS);
		normal(p[0], nv);
		hess(p[0], h);
		hs(h, hstar);
		curvaturaGaussiana[0] = curvatura_gaussiana(nv, h, hstar);
		glColor3f(-(curvaturaGaussiana[0] - 1) / 2, (curvaturaGaussiana[0] + 1) / 2, 0);
		//glNormal3fv(nv);
		glVertex3fv(p[0]);

		normal(p[1], nv);
		hess(p[1], h);
		hs(h, hstar);
		curvaturaGaussiana[1] = curvatura_gaussiana(nv, h, hstar);
		glColor3f( - (curvaturaGaussiana[1] - 1) / 2, (curvaturaGaussiana[1] + 1) / 2, 0);
		//glNormal3fv(nv);
		glVertex3fv(p[1]);

		normal(p[2], nv);
		hess(p[2], h);
		hs(h, hstar);
		curvaturaGaussiana[2] = curvatura_gaussiana(nv, h, hstar);
		glColor3f( - (curvaturaGaussiana[2] - 1) / 2, (curvaturaGaussiana[2] + 1) / 2, 0);
		//glNormal3fv(nv);
		glVertex3fv(p[2]);

		normal(p[3], nv);
		hess(p[3], h);
		hs(h, hstar);
		curvaturaGaussiana[3] = curvatura_gaussiana(nv, h, hstar);
		glColor3f( - (curvaturaGaussiana[3] - 1) / 2, (curvaturaGaussiana[3] + 1) / 2, 0);
		//glNormal3fv(nv);
		glVertex3fv(p[3]);
		glEnd();

		/*bool temgauss[4] = {false,false,false,false};
		if (curvaturaGaussiana[0] * curvaturaGaussiana[1] < 0) {
			xyzG[0][0] = (p[0][0] + p[1][0]) / 2;
			xyzG[0][1] = (p[0][1] + p[1][1]) / 2;
			xyzG[0][2] = (p[0][2] + p[1][2]) / 2;
			temgauss[0] = true;
		}
		if (curvaturaGaussiana[0] * curvaturaGaussiana[2] < 0) {
			xyzG[1][0] = (p[0][0] + p[2][0]) / 2;
			xyzG[1][1] = (p[0][1] + p[2][1]) / 2;
			xyzG[1][2] = (p[0][2] + p[2][2]) / 2;
			temgauss[1] = true;
		}
		if (curvaturaGaussiana[1] * curvaturaGaussiana[3] < 0) {
			xyzG[2][0] = (p[1][0] + p[3][0]) / 2;
			xyzG[2][1] = (p[1][1] + p[3][1]) / 2;
			xyzG[2][2] = (p[1][2] + p[3][2]) / 2;
			temgauss[2] = true;
		}
		if (curvaturaGaussiana[2] * curvaturaGaussiana[3] < 0) {
			xyzG[3][0] = (p[3][0] + p[2][0]) / 2;
			xyzG[3][1] = (p[3][1] + p[2][1]) / 2;
			xyzG[3][2] = (p[3][2] + p[2][2]) / 2;
			temgauss[3] = true;
		}
		if (temgauss[0] && temgauss[1]) {
			glColor3f(0, 0, 1);
			glBegin(GL_LINE);
			glVertex3fv(xyzG[0]);
			glVertex3fv(xyzG[1]);
			glEnd();
		}
		if (temgauss[2] && temgauss[1]) {
			glColor3f(0, 0, 1);
			glBegin(GL_LINE);
			glVertex3fv(xyzG[2]);
			glVertex3fv(xyzG[1]);
			glEnd();
		}
		if (temgauss[3] && temgauss[2]) {
			glColor3f(0, 0, 1);
			glBegin(GL_LINE);
			glVertex3fv(xyzG[3]);
			glVertex3fv(xyzG[2]);
			glEnd();
		}
		if (temgauss[3] && temgauss[1]) {
			glColor3f(0, 0, 1);
			glBegin(GL_LINE);
			glVertex3fv(xyzG[3]);
			glVertex3fv(xyzG[1]);
			glEnd();
		}*/
#endif
	}

}