#include "geometry/mandelbulb.h"
#include "utility.h"

const int nr_mandelbulb_params = 0;

/*
Params are:  
- There are no params, mandelbulb spawned at position zero
 */
double sdf_mandelbulb(const double vec_p[NR_VEC_ELEMENTS], double params[])
{
    double v__w[NR_VEC_ELEMENTS];
    double x = vec_p[0];
    double y = vec_p[1];
    double z = vec_p[2];

    double m = x*x + y*y + z*z;

	double dz = 1.5;
    
	for(int i = 0; i < 4; i++)
    {
        double m2 = m*m;
        double m4 = m2*m2;
		dz = 8.0*sqrt(m4*m2*m)*dz + 1.0;

        double x2 = x*x; double x4 = x2*x2;
        double y2 = y*y; double y4 = y2*y2;
        double z2 = z*z; double z4 = z2*z2;

        double a = x2 + z2;
        double a2 = a*a;
        double a4 = a2*a2;
        
        double b = 1.0/sqrt(a*a2*a4); // double b = (a == 0) ? 0.0 : 1.0/sqrt(a*a2*a4);
        double c = x4 + y4 + z4 - 6.0*y2*(z2 + x2) + 2.0*z2*x2;
        // double c = x4 + y4 + z4 - 6.0*y2*z2 - 6.0*x2*y2 + 2.0*z2*x2;
        double d = x2 - y2 + z2;
        
        double cb = c*b;

        x = vec_p[0] + 64.0*x*y*z*(x2 - z2)*d*(x4-6.0*x2*z2 + z4)*cb;
        // z = vec_p[2] +  -8.0*y*d*(x4*x4 - 28.0*x4*x2*z2 + 70.0*x4*z4 - 28.0*x2*z2*z4 + z4*z4)*c*b; // old
        z = vec_p[2] + 8.0*y*d*( 28.0*x2*z2*(x4 + z4)  - (70.0*x4 + z4)*z4 - x4*x4 )*cb;
        
        y = vec_p[1] - 16.0*y2*a*d*d + c*c;
        m = x*x + y*y + z*z;
		if( m > 256.0 )
            break;
    }

    return 0.25*log(m)*sqrt(m)/dz;
}


// double sdf_mandelbulb(const double vec_p[NR_VEC_ELEMENTS], double params[])
// {
//     double x = vec_p[0];
//     double y = vec_p[1];
//     double z = vec_p[2];

//     double m = x*x + y*y + z*z;

// 	double dz = 1.5;
    
// 	for(int i = 0; i < 4; i++)
//     {
//         double m2 = m*m;
//         double m4 = m2*m2;
// 		dz = 8.0*sqrt(m4*m2*m)*dz + 1.0;

//         double x2 = x*x; double x4 = x2*x2;
//         double y2 = y*y; double y4 = y2*y2;
//         double z2 = z*z; double z4 = z2*z2;

//         double h = x2 + z2;
//         double h2 = h*h;
//         double h4 = h2*h2;

//         double j = (h == 0) ? 0.0 : 1.0/sqrt( h4*h2*h );
//         // double j = 1.0/sqrt( h4*h2*h );
//         double k = x4 + y4 + z4 - 6.0*y2*z2 - 6.0*x2*y2 + 2.0*z2*x2;
//         double g = x2 - y2 + z2;

//         x = vec_p[0] +  64.0*x*y*z*(x2 - z2) * g * (x4 - 6.0*x2*z2 + z4)*k*j;

//         // z = vec_p[2] +  -8.0*y*g*(x4*x4 - 28.0*x4*x2*z2 + 70.0*x4*z4 - 28.0*x2*z2*z4 + z4*z4)*k*j;

//         z = vec_p[2] +  -8.0*y*g*(x4*x4 - 28.0*x4*x2*z2 + 70.0*x4*z4 - 28.0*x2*z2*z4 + z4*z4)*k*j;

//         // z = vec_p[2] +  8.0*y*g*(28.0*x2*z2*(x4 + z4) - 70.0*x4*z4 - z4*z4 - x4*x4)*k*j;

//         // z = vec_p[2] +  8.0*y*g*(28.0*x2*z2*(x4 + z4) - (70.0*x4 + z4)*z4 - x4*x4)*k*j;

//         // z = vec_p[2] +  -8.0*y*g*(x4*x4 + (70.0*z4- 28.0*x2*z2)*(x4 + z4))*k*j;

//         y = vec_p[1] + -16.0*y2*h*g*g + k*k;

//         m = x*x + y*y + z*z;
// 		if( m > 256.0 )
//             break;
//     }

//     return 0.25*log(m)*sqrt(m)/dz;
// }