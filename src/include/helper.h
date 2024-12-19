#ifndef HELPER_H
#define HELPER_H

#define IN3_2_GPM(x) ((x) * 0.0043)
#define LBS_2_N(x) ((x) * 4.4482)
#define PSI_2_PA(x) ((x) * 6894.0)
#define M2_2_in2(x) ((x) * 1550.0)
#define cbM2Gal(x) (x * 264.172)
#define M_sqr_2_in_sqr(x) (x * 1550.0)
#define GPS_2_M3S(x) (x/264.2)
//#define lerp(x, x1, y1, x2, y2) (y1 + (x - x1) * ((y2 - y1) / (x2 - x1)))

double clamp(double value, double upper, double lower);

#endif