#ifndef FILTER_H
#define FILTER_H


class Filter
{
public:
    Filter(int in_rate);
    void Start(double in_frequency, double in_sensitifity);
    double Next(double in_x);

private:
    int rate;
    double sensitifity;
    double prev_x;
    double prev_y;
    double a_0, a_1, b_1;
};

#endif // FILTER_H
