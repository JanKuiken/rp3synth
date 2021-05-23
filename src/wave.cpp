#include "wave.h"

#include <iostream>  // TODO: moet weer weg
#include <cmath>
#include <random>

#include "utils.h"

Wave::Wave(std::shared_ptr<VoiceGlobals> in_voice_globals)
{
    //std::cout << "Wave constructor called" << std::endl;

    voice_globals = in_voice_globals;
    rate = voice_globals->rate;
    filter_1 = std::unique_ptr<Filter>(new Filter(rate));
    filter_2 = std::unique_ptr<Filter>(new Filter(rate));

    // strange stuff from https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(-1.0, 1.0);
    for (int i = 0; i < INT_PHI_MAX; i++) {
        wave_noise[i] = dis(gen);
    }

    //logger = std::make_unique<Logger<double>>("log_wave", std::size_t(1000000));
}

Wave::~Wave()
{
    //std::cout << "Wave destructor called" << std::endl;
}

bool Wave::isActive() {
    return active;
}

void Wave::Start(double in_frequency, double in_detune, const Waveform in_wf) {

    const double halve_note = pow(2.0, 1.0/12.0);

    phi = 0.0;
    phi_step =   M_PI
               * in_frequency
               * pow(halve_note, in_detune * 2.0 - 1.0)
               / static_cast<double>(rate);
    int_phi = 0;
    waveform = in_wf;
    pitch_sensitivity = 0.0;

    active = true;

    filter_1->Start(1.50 * in_frequency, "lowpass", 1.0);
    filter_2->Start(0.75 * in_frequency, "highpass", 1.0);
}


double Wave::Next(bool in_stop) {

    double retval = 0.0;
    double factor = phi / (2 * M_PI);

    switch (waveform) {
    case sine:
        retval = sin(phi);
    break;
    case square:
        retval = (factor < 0.5) ? 1.0 : -1.0;
    break;
    case triangle:
        if (factor < 0.25) {
            retval = factor * 4.0;
        } else if (factor < 0.75) {
            retval = 2.0 - factor * 4.0;
        } else {
            retval = -4.0 + factor * 4.0;
        }
    break;
    case sawtooth:
        if (factor < 0.5) {
            retval = factor * 2.0;
        } else {
            retval = -2.0 + factor * 2.0;
        }
    break;
    case noise:
        retval = wave_noise[int_phi];
        retval = filter_1->Next(retval);
        retval = filter_2->Next(retval);
    break;
    }

    phi += phi_step;
    if (phi >= 2 * M_PI) {
        phi -= 2 * M_PI * pow(2.0, -voice_globals->pitch * pitch_sensitivity);

        if (in_stop) {
            active = false;
        }
        // apply filters
    }
    int_phi += 1;
    if (int_phi >= INT_PHI_MAX) {
        int_phi  = 0;
    }

//    if (!active) {
//        // is stopped return zeros
//        retval = 0.0;
//    }


    // temporaly: test rough clipping amplifier
    // double amplifier_factor = 1.0 + 5.0 * bounds_limit(voice_globals->pitch, 0.0, 1.0);
    // retval = bounds_limit(amplifier_factor * retval, -1.0, 1.0);
    //logger->Log(retval);
    return retval;
}

