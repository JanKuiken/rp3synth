#ifndef WAVE_H
#define WAVE_H

#include <string>
#include <random>
#include <memory>

#include "voiceglobals.h"
#include "filter.h"


enum WaveForm { sine,
                square,
                triangle,
                sawtooth,
                noise
              };

class Wave
{
public:
    Wave(std::shared_ptr<VoiceGlobals> in_voice_globals);

    void Start(double in_frequency, const std::string& in_wave);
    double Next();


private:

    WaveForm StringToWaveform(std::string in_wave);

    int rate;            // samples per seconds
    std::shared_ptr<VoiceGlobals> voice_globals;
    std::unique_ptr<Filter> filter_1;
    std::unique_ptr<Filter> filter_2;

    WaveForm  waveform;
    double pitch_sensitivity;

    double phi;
    double phi_step;

    std::mt19937 gen;
    std::uniform_real_distribution<> dis;
};

#endif // WAVE_H
