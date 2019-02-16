#include "wavesine.h"

#include <math.h>

WaveSine::WaveSine(int in_rate)
{
    rate = in_rate;
}

void WaveSine::Start(double in_frequency) {
    phi = 0.0;
    phi_step = M_PI * in_frequency / (double)rate;
}

double WaveSine::Next() {
    double retval = sin(phi);
    phi += phi_step;
    if (phi >= M_2_PI) {
        phi -= M_2_PI;
    }
    return retval;
}

