#include <stdio.h>
#include <math.h>
#include "defs.h"

const double PI = 3.141592;

const double H = 6.0; 				// Kernel radius
const double HSQ = H * H;			// Square of the radius
const double G = 12000.0 * -9.8; 	// External gravitation forces;
const double REST_DENS = 1000.0; 	// Rest density
const double GAS_CONST = 2000.0;	// Const for equation of state
const double VISC = 350.0f;			// Viscosity
const double DT = 0.00000002f;			// Integration timestep
const double EPS = 1.0;
const double BOUND_DAMPING = -0.8;

// Smoothing kernals defined in Muller and their gradients;
double POLY6;
double SPIKY_GRAD;
double VISC_LAP;

void ComputeDensityPressure(struct Particle *p, int count)
{
	for (int i = 0; i < count; i++)
	{
		p[i].rho = 0.0;
		for (int j = 0; j < count; j++)
		{
			double rx = p[j].x - p[i].x;
			double ry = p[j].y - p[i].y;
			float r2 = rx * rx + ry * ry;
			if (r2 < H * H)
			{
				p[i].rho += p[j].m * POLY6 * pow(HSQ - r2, 3);
			}
		}
		p[i].p = GAS_CONST * (p[i].rho - REST_DENS);
	}
}

void ComputeForces(struct Particle *p, int count)
{
	for (int i = 0; i < count; i++)
	{
		double fpx = 0.0f;
		double fpy = 0.0f;
		double fvx = 0.0f;
		double fvy = 0.0f;
		
		for (int j = 0; j < count; j++)
		{
			if (i == j)
			{
				continue;
			}
			
			double rx = p[j].x - p[i].x;
			double ry = p[j].y - p[i].y;
			double r = sqrt(rx * rx + ry * ry);
			
			if (r < H)
			{
				// Compute pressure force contribution
				fpx += -rx / r * p[j].m * (p[i].p + p[j].p) / (2.0 * p[j].rho) * SPIKY_GRAD * pow(H - r, 2);
				fpy += -ry / r * p[j].m * (p[i].p + p[j].p) / (2.0 * p[j].rho) * SPIKY_GRAD * pow(H - r, 2);
				// Compute viscosity force contribution
				fvx += VISC * p[j].m * (p[j].u - p[i].u) / p[j].rho * VISC_LAP * (H - r);
				fvy += VISC * p[j].m * (p[j].v - p[i].v) / p[j].rho * VISC_LAP * (H - r);
			}
		}
		double fgrav = G * p[i].rho;
		p[i].fx = fpx + fvx;
		p[i].fy = fpy + fvy + fgrav;
	}
}

void Integrate(struct Particle *p, int count)
{
	for (int i = 0; i < count; i++)
	{
		// Forward Euler integration
		p[i].u += DT * p[i].fx / p[i].rho;
		p[i].v += DT * p[i].fy / p[i].rho;
		
		//printf("%f, %f\n", p[i].u, p[i].v);
		
		// Enforce boundary conditions
		if (p[i].x - EPS < 0.0)
		{
			p[i].u *= BOUND_DAMPING;
			p[i].x = EPS;
		}
		if (p[i].x + EPS > width)
		{
			p[i].u *= BOUND_DAMPING;
			p[i].x = width - EPS;
		}
		if (p[i].y - EPS < 0.0)
		{
			p[i].v *= BOUND_DAMPING;
			p[i].y = EPS;
		}
		if (p[i].y + EPS > height)
		{
			p[i].v *= BOUND_DAMPING;
			p[i].y = height - EPS;
		}
	}
}

void Step(struct Particle *p, int count)
{
	ComputeDensityPressure(p, count);
	ComputeForces(p, count);
	Integrate(p, count);
}