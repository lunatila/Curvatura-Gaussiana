#pragma once
#include "Function.h"

class MS
{
	float xmin, xmax, ymin, ymax;
	int n;

	Function F;
public:
	MS(float xm, float ym, float XM, float YM, int p) {
		xmin = xm;
		xmax = XM;
		ymin = ym;
		ymax = YM;
		n = p;
	}

	float c(float *xyz) {
		float grad[3];
		float h[9];
		float hstar[9];

		F.normal(xyz, grad);
		F.hess(xyz, h);
		F.hs(h, hstar);

		float curvaturaG = F.curvatura_gaussiana(grad, h, hstar);

		return curvaturaG;
	}

	void visualiza_curva();

	void curva(float p[4][3]);
};
