#include "filter.h"

#include <cmath>

#include "utils.h"

Filter::Filter(int in_rate)
{
    rate = in_rate;
}

void Filter::Start(double in_frequency, double in_sensitifity)
{
    // IIR filter constants for a low pass filter
    sensitifity = bounds_limit(in_sensitifity, 0.0, 1.0);
    double fx = exp(-2.0 * M_PI * in_frequency / rate);
    a_0 = 1.0 - fx;
    a_1 = 0.0;
    b_1 = fx;
    prev_x = 0.0;
    prev_y = 0.0;
}

double Filter::Next(double current_x)
{
    double current_y =   a_0 * current_x
                       + a_1 * prev_x
                       + b_1 * prev_y ;
    prev_x = current_x;
    prev_y = current_y;
    return sensitifity * current_y + (1.0 - sensitifity) * current_x;
}
