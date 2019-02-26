#ifndef FILTER_H
#define FILTER_H

#include <string>

enum FilterType { lowpass,
                  highpass
                 };

class Filter
{
public:
    Filter(int in_rate);
    void Start(double in_frequency, std::string in_type, double in_sensitifity);
    double Next(double in_x);

private:

    FilterType StringToFilterType(std::string in_type);

    int rate;
    FilterType filtertype;
    double sensitifity;
    double prev_x;
    double prev_y;
    double a_0, a_1, b_1;
};

#endif // FILTER_H
