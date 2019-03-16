#include "filter.h"

#include <cmath>

#include "utils.h"

Filter::Filter(int in_rate)
{
    rate = in_rate;
}

void Filter::Start(double in_frequency, std::string in_type, double in_sensitifity)
{
    // IIR filter constants for a low pass filter
    sensitifity = bounds_limit(in_sensitifity, 0.0, 1.0);
    filtertype = StringToFilterType(in_type);
    double fx = exp(-2.0 * M_PI * in_frequency / rate);
    switch (filtertype) {
    case lowpass:
        a_0 = 1.0 - fx;
        a_1 = 0.0;
        b_1 = fx;
        break;
    case highpass:
        a_0 =   (1.0 + fx) / 2.0;
        a_1 = - (1.0 + fx) / 2.0;
        b_1 = fx;
        break;
    default:
        a_0 = 1.0;
        a_1 = 0.0;
        b_1 = 0.0;
        break;
    }
    prev_x = 0.0;
    prev_y = 0.0;
}

FilterType Filter::StringToFilterType(std::string in_type) {
    FilterType ft = lowpass; // default, if no valid string is provided
    if (in_type == "lowpass")  ft = lowpass;
    if (in_type == "highpass") ft = highpass;
    return ft;
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
