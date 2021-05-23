
#include "waveform.h"

Waveform StringToWaveform(std::string in_wave)
{
    Waveform wf = sine; // default, if no valid string is provided
    if (in_wave == "sine")     wf = sine;
    if (in_wave == "square")   wf = square;
    if (in_wave == "triangle") wf = triangle;
    if (in_wave == "sawtooth") wf = sawtooth;
    if (in_wave == "noise")    wf = noise;
    return wf;
}

std::string WaveformToString(Waveform in_wf)
{
    std::string wave = "";
    if (in_wf == sine)     wave="sine";
    if (in_wf == square)   wave="square";
    if (in_wf == triangle) wave="triangle";
    if (in_wf == sawtooth) wave="sawtooth";
    if (in_wf == noise)    wave="noise";
    return wave;
}

Waveform CycleWaveformNext(Waveform in_wf){
    switch (in_wf) {
    case sine:     return square;
    case square:   return triangle;
    case triangle: return sawtooth;
    case sawtooth: return noise;
    case noise:    return sine;
    }
    return sine; // only to prevent compiler warning
}
