#ifndef VOICESETTINGS_H
#define VOICESETTINGS_H

#include <string>

class VoiceSettings
{
public:
    VoiceSettings(const std::string& in_settings_file);

    void Load();
    void Save();

    // web interface...
    // smooth transition to new settings...

    // eerst maar even public voor het gemak, later wellicht voor snelheid
    std::string waveform_1;
    std::string waveform_2;

    double gain_1;
    double attack_1;
    double decay_1;
    double sustain_1;
    double release_1;

    double gain_2;
    double attack_2;
    double decay_2;
    double sustain_2;
    double release_2;

private:
    std::string settings_file;
};

#endif // VOICESETTINGS_H
