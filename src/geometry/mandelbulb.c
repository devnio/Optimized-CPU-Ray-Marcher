#include "geometry/mandelbulb.h"
#include "utility.h"

const int nr_mandelbulb_params = 0;

/*
Params are:  
- There are no params, mandelbulb spawned at position zero
 */
double sdf_mandelbulb(Vec3 p, double params[])
{
    Vec3 w = p;
    double m = vec_dot(&w,&w);

	double dz = 1.5;
    
	for(int i = 0; i < 4; i++)
    {
        double m2 = m*m;
        double m4 = m2*m2;
		dz = 8.0*sqrt(m4*m2*m)*dz + 1.0;

        double x = w.x; double x2 = x*x; double x4 = x2*x2;
        double y = w.y; double y2 = y*y; double y4 = y2*y2;
        double z = w.z; double z2 = z*z; double z4 = z2*z2;

        double k3 = x2 + z2;
        double k2 = (k3 == 0) ? 0.0 : 1.0/sqrt( k3*k3*k3*k3*k3*k3*k3 );
        double k1 = x4 + y4 + z4 - 6.0*y2*z2 - 6.0*x2*y2 + 2.0*z2*x2;
        double k4 = x2 - y2 + z2;

        w.x = p.x +  64.0*x*y*z*(x2-z2)*k4*(x4-6.0*x2*z2+z4)*k1*k2;
        w.y = p.y -  16.0*y2*k3*k4*k4 + k1*k1;
        w.z = p.z -  8.0*y*k4*(x4*x4 - 28.0*x4*x2*z2 + 70.0*x4*z4 - 28.0*x2*z2*z4 + z4*z4)*k1*k2;

        m = vec_dot(&w,&w);
		if( m > 256.0 )
            break;
    }

    return 0.25*log(m)*sqrt(m)/dz;
}