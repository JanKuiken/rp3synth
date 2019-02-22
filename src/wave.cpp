#include "wave.h"

#include <cmath>

Wave::Wave(int in_rate)
{
    rate = in_rate;
}

void Wave::Start(double in_frequency) {
    phi = 0.0;
    phi_step = M_PI * in_frequency / (double)rate;
}

double Wave::Next() {
    double retval = sin(phi);
    phi += phi_step;
    if (phi >= 2 * M_PI) {
        phi -= 2 * M_PI;
    }
    return retval;
}

