#include "MS.h"
#include <GL/glut.h>

void MS::visualiza_curva() {
	float dx = (F.xmax - F.xmin) / F.n;
	float dy = (F.ymax - F.ymin) / F.n;
	float dz = (F.zmax - F.zmin) / F.n;
	float p[4][3];
	p[0][0] = F.xmin;
	for (int i = 0; i < n; i++) {
		p[0][1] = F.ymin;
		for (int j = 0; j < n; j++) {
			p[0][2] = F.zmin;
			for (int k = 0; k < n; k++) {
				p[0][0] += dz;

			}
			p[0][1] += dy;
		}
		p[0][2] += dx;
	}



	/*float dx = (xmax - xmin) / n;
	float dy = (ymax - ymin) / n;
	float x = xmin;
	float y;*/


	//for (int i = 0; i < n; i++) {
	//	y = ymin;
	//	for (int j = 0; j < n; j++) {
	//		curva(x, y, dx, dy);
	//		glColor3f(0, 0, 1);
	//		/*glBegin(GL_LINE_LOOP);
	//		glVertex2f(x, y);
	//		glVertex2f(x + dx, y);
	//		glVertex2f(x + dx, y + dy);
	//		glVertex2f(x, y + dy);
	//		glEnd();*/
	//		y += dy;
	//	}
	//	x += dx;
	//}
}

void MS::curva(float p[4][3]) {
	float s[4];

	s[0] = c(p[0]);
	s[1] = c(p[1]);
	s[2] = c(p[2]);
	s[3] = c(p[3]);

	int ind = (s[0] < 0) ? 1 : 0; //indice dos vertices
	ind += (s[1] < 0) ? 2 : 0;
	ind += (s[2] < 0) ? 4 : 0;
	ind += (s[3] < 0) ? 8 : 0;

	int e1, e2; //indice das arestas

	switch (ind) {
	case 1:
	case 14:
		e1 = 0;
		e2 = 3;
		break;
	case 2:
	case 13:
		e1 = 0;
		e2 = 1;
		break;
	case 4:
	case 11:
		e1 = 1;
		e2 = 2;
		break;
	case 8:
	case 7:
		e1 = 2;
		e2 = 3;
		break;
	case 3:
	case 12:
		e1 = 1;
		e2 = 3;
		break;
	case 6:
	case 9:
		e1 = 0;
		e2 = 2;
		break;
	case 5:
	case 10:
		break;
	}

	if (ind > 0 && ind < 15) {

		float xc[4], yc[4], zc[4];

		xc[0] = p[0][0]; xc[1] = p[1][0]; xc[2] = p[2][0]; xc[3] = p[3][0];
		yc[0] = p[0][1]; yc[1] = p[1][1]; yc[2] = p[2][1]; yc[3] = p[3][1];
		zc[0] = p[0][2]; yc[1] = p[1][2]; yc[2] = p[2][2]; yc[3] = p[3][2];

		if (ind != 5 && ind != 10) {

			float px[2], py[2], pz[2];

			float t = -s[e1] / (s[e1 + 1] - s[e1]);
			px[0] = xc[e1] + t * (xc[e1 + 1] - xc[e1]);
			py[0] = yc[e1] + t * (yc[e1 + 1] - yc[e1]);
			pz[0] = zc[e1] + t * (zc[e1 + 1] - zc[e1]);

			t = -s[e2] / (s[(e2 + 1) % 4] - s[e2]);
			px[1] = xc[e2] + t * (xc[(e2 + 1) % 4] - xc[e2]);
			py[1] = yc[e2] + t * (yc[(e2 + 1) % 4] - yc[e2]);
			pz[1] = zc[e2] + t * (zc[(e2 + 1) % 4] - zc[e2]);

			glColor3f(1, 0, 0);
			glBegin(GL_LINES);
			glVertex3f(px[0], py[0], pz[0]);
			glVertex3f(px[1], py[1], pz[1]);
			glEnd();
		}
		else {
			float pxx[4], pyy[4], pzz[4];

			float t = -s[0] / (s[1] - s[0]);
			pxx[0] = xc[0] + t * (xc[1] - xc[0]);
			pyy[0] = yc[0] + t * (yc[1] - yc[0]);
			pzz[0] = zc[0] + t * (zc[1] - zc[0]);

			t = -s[1] / (s[2] - s[1]);
			pxx[1] = xc[1] + t * (xc[2] - xc[1]);
			pyy[1] = yc[1] + t * (yc[2] - yc[1]);
			pzz[1] = zc[1] + t * (zc[2] - zc[1]);

			t = -s[2] / (s[2 + 1] - s[2]);
			pxx[2] = xc[2] + t * (xc[3] - xc[2]);
			pyy[2] = yc[2] + t * (yc[3] - yc[2]);
			pzz[2] = zc[2] + t * (zc[3] - zc[2]);

			t = -s[3] / (s[0] - s[3]);
			pxx[3] = xc[3] + t * (xc[0] - xc[3]);
			pyy[3] = yc[3] + t * (yc[0] - yc[3]);
			pzz[3] = zc[3] + t * (zc[0] - zc[3]);

			if ((s[0] * s[2] - s[1] * s[3]) > 0) {
				glColor3f(1, 0, 0);
				glBegin(GL_LINES);
				glVertex3f(pxx[0], pyy[0], pzz[0]);
				glVertex3f(pxx[1], pyy[1], pzz[1]);
				glEnd();
				glBegin(GL_LINES);
				glVertex3f(pxx[2], pyy[2], pzz[2]);
				glVertex3f(pxx[3], pyy[3], pzz[3]);
				glEnd();
			}
			else {
				glColor3f(1, 0, 0);
				glBegin(GL_LINES);
				glVertex3f(pxx[0], pyy[0], pzz[0]);
				glVertex3f(pxx[3], pyy[3], pzz[3]);
				glEnd();
				glBegin(GL_LINES);
				glVertex3f(pxx[2], pyy[2], pzz[2]);
				glVertex3f(pxx[1], pyy[1], pzz[1]);
				glEnd();
			}
		}
	}
}