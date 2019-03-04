#include "wave.h"

#include <iostream>  // TODO: moet weer weg
#include <cmath>
#include <random>

#include "utils.h"

Wave::Wave(std::shared_ptr<VoiceGlobals> in_voice_globals)
{
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
}

void Wave::Start(double in_frequency, const std::string& in_wave) {
    phi = 0.0;
    phi_step = M_PI * in_frequency / (double)rate;
    int_phi = 0;
    waveform = StringToWaveform(in_wave);
    pitch_sensitivity = 0.0;

    filter_1->Start(1.50 * in_frequency, "lowpass", 1.0);
    filter_2->Start(0.75 * in_frequency, "highpass", 1.0);
}

WaveForm Wave::StringToWaveform(std::string in_wave) {
    WaveForm wf = sine; // default, if no valid string is provided
    if (in_wave == "sine")     wf = sine;
    if (in_wave == "square")   wf = square;
    if (in_wave == "triangle") wf = triangle;
    if (in_wave == "sawtooth") wf = sawtooth;
    if (in_wave == "noise")    wf = noise;
    return wf;
}

double Wave::Next() {
    double retval;
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
        break;
    }
    phi += phi_step;
    if (phi >= 2 * M_PI) {
        phi -= 2 * M_PI * pow(2.0, -voice_globals->pitch * pitch_sensitivity);
    }
    int_phi += 1;
    if (int_phi >= INT_PHI_MAX) {
        int_phi  = 0;
    }

    // apply filters
    retval = filter_1->Next(retval);
    retval = filter_2->Next(retval);

    // temporaly: test rough clipping amplifier
    double amplifier_factor = 1.0 + 5.0 * bounds_limit(voice_globals->pitch, 0.0, 1.0);
    retval = bounds_limit(amplifier_factor * retval, -1.0, 1.0);
    return retval;
}

