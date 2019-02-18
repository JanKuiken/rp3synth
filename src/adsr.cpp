#include "adsr.h"

ADSR::ADSR(int in_rate) {
    rate    = in_rate;
    state   = idle;
    stopped = false;
}

bool ADSR::isActive() {
    return state != idle;
}

void ADSR::Start(double in_attack,
                 double in_decay,
                 double in_sustain,
                 double in_release) {

    attack_ticks  = rate * in_attack;
    decay_ticks   = rate * in_decay;
    sustain       =        in_sustain;
    release_ticks = rate * in_release;

    state   = key_attack;
    stopped = false;
}

double ADSR::Next(bool in_stop) {

    stopped = in_stop;
    double retval = 0.0;

    switch (state) {

    case idle:
        retval = 0.0;
        break;

    case key_attack:
        retval = (double)counter / (double)attack_ticks;
        counter++;
        if (counter > attack_ticks) {
            state = key_decay;
            counter = 0;
        }
        break;

    case key_decay:
        retval = 1.0 - (1.0 - sustain) * ((double)counter / (double)decay_ticks);
        counter++;
        if (counter > decay_ticks) {
            state = key_sustain;
            counter = 0;
        }
        break;

    case key_sustain:
        retval = sustain;
        if (stopped) {
            state = key_release;
            counter = 0;
        }
        break;

    case key_release:
        retval = sustain * (1.0 - (double)counter / (double)release_ticks);
        counter++;
        if (counter >= release_ticks) {
            state = idle;
        }
        break;
    }
    return retval;
}

