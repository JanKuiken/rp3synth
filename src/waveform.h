#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <string>

enum Waveform { sine,
                square,
                triangle,
                sawtooth,
                noise
              };


Waveform StringToWaveform(std::string in_wave);
std::string WaveformToString(Waveform in_wf);
Waveform CycleWaveformNext(Waveform in_wf);


#endif // WAVEFORM_H
