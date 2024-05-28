#ifndef INTERP_H 
#define INTERP_H
#include <vector>

struct DataPoint {
    double x;
    double y;
};

//linear interpolation
double lerp(double x, double x1, double y1, double x2, double y2);
// Custom comparator function to sort the data points by x values
bool compareDataPoints(const DataPoint& a, const DataPoint& b);
//non-linear interpolation function
double interpolate(const std::vector<DataPoint>& data, double x);

#endif
