#ifndef WAVE_H
#define WAVE_H

#include <string>
#include <random>

enum WaveForm { sine,
                square,
                triangle,
                sawtooth,
                noise
              };

class Wave
{
public:
    Wave(int in_rate);

    void Start(double in_frequency, const std::string& in_wave);
    double Next();


private:

    WaveForm StringToWaveform(std::string in_wave);

    int rate;            // samples per seconds
    WaveForm  waveform;
    double phi;
    double phi_step;

    std::mt19937 gen;
    std::uniform_real_distribution<> dis;
};

#endif // WAVE_H
