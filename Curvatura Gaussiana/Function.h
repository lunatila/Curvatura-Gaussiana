#pragma once
#include <cmath>
#include <GL/glut.h>
#include <iostream>

using namespace std;

class Function
{
public:
	float xmin, xmax, ymin, ymax, zmin, zmax;
	int n;

	int GaussOrMedian = 0;
	Function(float xm = -1, float xM = 1,
		float ym = -1, float yM = 1,
		float zm = -1, float zM = 1, int p = 10)
	{
		xmin = xm;
		xmax = xM;
		ymin = ym;
		ymax = yM;
		zmin = zm;
		zmax = zM;
		n = p;
	}

	float f(float x, float y, float z)
	{
		return x * x + cos(x) * y * y - z * y - 0.82;
	}

	void normal(float* xyz, float* nv) 
	{ 	
		float x = xyz[0];
		float y = xyz[1];
		float z = xyz[2];

		nv[0] = 2 * x - sin(x) * y * y; // df/dx
		nv[1] = cos(x) * 2 * y - z; // df/dy
		nv[2] = -y; // df/dz
	}

	void hess(float* xyz, float* hessF)
	{
		float x = xyz[0];
		float y = xyz[1];
		float z = xyz[2];

		hessF[0] = 2 - cos(x) * y * y;	// Fxx = 0
		hessF[1] = -sin(x) * 2 * y;		// Fxy = 1
		hessF[2] = 0;					// Fxz = 2
		hessF[3] = -sin(x) * 2 * y;		// Fyx = 3
		hessF[4] = cos(x) * 2;			// Fyy = 4
		hessF[5] = -1;					// Fyz = 5
		hessF[6] = 0;					// Fzx = 6
		hessF[7] = -1;					// Fzy = 7
		hessF[8] = 0;					// Fzz = 8
	}

	void hs(float* hess, float* hess_star)
	{
		hess_star[0] = hess[4] * hess[8] - hess[5] * hess[7]; // ok
		hess_star[1] = hess[5] * hess[6] - hess[3] * hess[8]; // ok
		hess_star[2] = hess[3] * hess[7] - hess[4] * hess[6]; // ok
		hess_star[3] = hess[2] * hess[7] - hess[1] * hess[8]; // ok
		hess_star[4] = hess[0] * hess[8] - hess[2] * hess[6]; // ok
		hess_star[5] = hess[1] * hess[6] - hess[0] * hess[7]; // ok
		hess_star[6] = hess[1] * hess[5] - hess[2] * hess[4]; // ok
		hess_star[7] = hess[3] * hess[2] - hess[0] * hess[5]; // ok
		hess_star[8] = hess[0] * hess[4] - hess[1] * hess[3]; // ok
	}
	float curvatura_gaussiana(float* grad, float* hess, float* hess_star)
	{
		float modulo_grad = sqrt(grad[0] * grad[0] + grad[1] * grad[1] + grad[2] * grad[2]);

		return ((grad[0] * hess_star[0] + grad[1] * hess_star[3] + grad[2] * hess_star[6]) * grad[0] +
			(grad[0] * hess_star[1] + grad[1] * hess_star[4] + grad[2] * hess_star[7]) * grad[1] +
			(grad[0] * hess_star[2] + grad[1] * hess_star[5] + grad[2] * hess_star[8]) * grad[2]) /
			(modulo_grad * modulo_grad * modulo_grad * modulo_grad);
	}
	float curvatura_media(float* grad, float* hess)
	{
		float modulo_grad = sqrt(grad[0] * grad[0] + grad[1] * grad[1] + grad[2] * grad[2]);
		float traceH = hess[0] + hess[4] + hess[8];
		float GxHxGt = (grad[0] * hess[0] + grad[1] * hess[3] + grad[2] * hess[6]) * grad[0] +
			(grad[0] * hess[1] + grad[1] * hess[4] + grad[2] * hess[7]) * grad[1] +
			(grad[0] * hess[2] + grad[1] * hess[5] + grad[2] * hess[8]) * grad[2];

		return (GxHxGt - modulo_grad * modulo_grad * traceH) / (2 * modulo_grad * modulo_grad * modulo_grad);
	}
	

	void display_surface();
	void tetraedro(float* x, float* y, float* z, float* w);
};

