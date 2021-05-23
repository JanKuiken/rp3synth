#include "adsr.h"

#include <iostream>  // TODO: moet weer weg
#include <algorithm>

const double min_time = 0.001;

ADSR::ADSR(int in_rate) {
    rate    = in_rate;
    state   = idle;
    stopped = false;

    // logger = std::make_unique<Logger<double>>("log_adsr", std::size_t(1000));
}

ADSR::~ADSR() {
}

bool ADSR::isActive() {
    return state != idle;
}

void ADSR::Start(double in_attack,
                 double in_decay,
                 double in_sustain,
                 double in_release) {

    // sanity checks
    double attack  = std::max(0.0, in_attack);
    double decay   = std::max(0.0, in_decay);
    double release = std::max(0.0, in_release);

    sustain_level  = std::min(1.0, std::max(0.0, in_sustain));

    attack_rate  = 1.0                   / (rate * (min_time + attack));
    decay_rate   = (1.0 - sustain_level) / (rate * (min_time + decay));
    release_rate = (sustain_level)       / (rate * (min_time + release));

    state       = key_attack;
    stopped     = false;
    current_val = 0.0;

    // fix to leading avoid tick if a == 0 and d == 0
    if (attack == 0.0 && decay == 0.0) {
        current_val = sustain_level;
        state = key_sustain;
    } else if (attack == 0.0) {
        current_val = 1.0;
        state = key_decay;
    }
}

double ADSR::Next(bool in_stop) {

    if (in_stop) {
        stopped = true;
        state = key_release;
    }

    switch (state) {

    case idle:
        current_val = 0.0;
        break;

    case key_attack:
        current_val += attack_rate;
        if (current_val >= 1.0) {
            current_val = 1.0;
            state = key_decay;
        }
        break;

    case key_decay:
        current_val -= attack_rate;
        if (current_val <= sustain_level) {
            current_val = sustain_level;
            state = key_sustain;
        }
        break;

    case key_sustain:
        current_val = sustain_level;
        break;

    case key_release:
        current_val -= release_rate;
        if (current_val <= 0.0) {
            current_val = 0.0;
            state = idle;
            //std::cout << "release val " << current_val << " " << state << std::endl;
        }
        break;
    }

    //logger->Log(retval);
    return current_val;
}
