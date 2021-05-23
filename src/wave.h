#ifndef WAVE_H
#define WAVE_H

#include <random>
#include <memory>

#include "voiceglobals.h"
#include "waveform.h"
#include "filter.h"

#include "logger.h"

const int INT_PHI_MAX = 123456; //quite an arbitrary number


class Wave
{
public:
    Wave(std::shared_ptr<VoiceGlobals> in_voice_globals);
    ~Wave();
    bool isActive();
    void Start(double in_frequency, double indetune, const Waveform in_wave);
    double Next(bool in_stop);
    void NextWaveForm();


private:

    Waveform StringToWaveform(Waveform in_wf);

    int rate;            // samples per seconds
    std::shared_ptr<VoiceGlobals> voice_globals;
    std::unique_ptr<Filter> filter_1;
    std::unique_ptr<Filter> filter_2;

    Waveform  waveform;
    double pitch_sensitivity;

    bool active;
    double phi;
    double phi_step;
    int int_phi;
    double wave_noise[INT_PHI_MAX];

    std::unique_ptr<Logger<double>> logger;
};

#endif // WAVE_H
