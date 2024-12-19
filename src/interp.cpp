#include <iostream>
#include <algorithm>
#include "interp.h"
#include "helper.h"

double lerp(double x, double x1, double y1, double x2, double y2, bool is_percent)
{
    double value = (y1 + (x - x1) * ((y2 - y1) / (x2 - x1)));

    if(is_percent)
    {
        value = clamp(value, 1.0, 0.0);
    }

    return value;
}

bool compareDataPoints(const DataPoint& a, const DataPoint& b) {
    return a.x < b.x;
}

double interpolate(const std::vector<DataPoint>& data, double x) 
{
    // Sort the data points by x values
    std::vector<DataPoint> sortedData = data;
    std::sort(sortedData.begin(), sortedData.end(), compareDataPoints);
    
    // Find the nearest data points
    auto upper = std::upper_bound(sortedData.begin(), sortedData.end(), x, [](double val, const DataPoint& dataPoint) {
        return val < dataPoint.x;
    });
    
    // If x is smaller than the smallest x value, return the y value of the first data point
    if (upper == sortedData.begin()) {
        return sortedData.front().y;
    }
    
    // If x is larger than the largest x value, return the y value of the last data point
    if (upper == sortedData.end()) {
        return sortedData.back().y;
    }
    
    // Get the indices of the nearest data points
    size_t upperIndex = std::distance(sortedData.begin(), upper);
    size_t lowerIndex = upperIndex - 1;
    
    const DataPoint& upperPoint = sortedData[upperIndex];
    const DataPoint& lowerPoint = sortedData[lowerIndex];
    
    // Perform non-linear interpolation using the nearest data points
    double t = (x - lowerPoint.x) / (upperPoint.x - lowerPoint.x);
    
    // Perform interpolation (in this example, using a quadratic interpolation)
    double interpolatedValue = lowerPoint.y + t * (t - 1) * (upperPoint.y - lowerPoint.y);
    
    return interpolatedValue;
}
