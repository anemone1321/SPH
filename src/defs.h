extern int width;
extern int height;
extern double POLY6;
extern double SPIKY_GRAD;
extern double VISC_LAP;
extern const double H;
extern const double PI;

struct Particle
{
	double x, y;
	double u, v;
	double m;
	double fx, fy;
	double rho;
	double p;
};

void Step(struct Particle *p, int count);
