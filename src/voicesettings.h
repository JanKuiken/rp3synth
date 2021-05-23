#ifndef VOICESETTINGS_H
#define VOICESETTINGS_H

#include <string>

#include "waveform.h"

class VoiceSettings
{
public:
    VoiceSettings(const std::string& in_settings_file);
    ~VoiceSettings();

    void Load();
    void Save();
    void HandleKnob(unsigned int param, int value);
    void HandleButton(int note);

    // web interface...
    // smooth transition to new settings...

    // eerst maar even public voor het gemak, later wellicht voor snelheid
    Waveform waveform_1;
    Waveform waveform_2;
    double detune_1;
    double detune_2;

    double attack_1;
    double decay_1;
    double sustain_1;
    double release_1;

    double attack_2;
    double decay_2;
    double sustain_2;
    double release_2;

    double chorus_gain;
    double chorus_depth;
    double chorus_frequency;

    double master_volume;

private:
    std::string settings_file;
};

#endif // VOICESETTINGS_H
