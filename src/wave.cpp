#include "wave.h"

#include <cmath>

Wave::Wave(std::shared_ptr<VoiceGlobals> in_voice_globals)
{
    voice_globals = in_voice_globals;
    rate = voice_globals->rate;
    filter = std::unique_ptr<Filter>(new Filter(rate));

    // strange stuff from https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    gen = std::mt19937(rd()); //Standard mersenne_twister_engine seeded with rd()
    dis = std::uniform_real_distribution<>(-1.0, 1.0);
}

void Wave::Start(double in_frequency, const std::string& in_wave) {
    phi = 0.0;
    phi_step = M_PI * in_frequency / (double)rate;
    waveform = StringToWaveform(in_wave);
    pitch_sensitivity = 0.1;

    filter->Start(2.0 * in_frequency, 1.0);
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
            retval = dis(gen);
        break;
    }
    phi += phi_step;
    if (phi >= 2 * M_PI) {
        phi -= 2 * M_PI * pow(2.0, -voice_globals->pitch * pitch_sensitivity);
    }

    // apply filter
    retval = filter->Next(retval);
    return retval;
}

