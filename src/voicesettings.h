#ifndef VOICESETTINGS_H
#define VOICESETTINGS_H

#include <string>

class VoiceSettings
{
public:
    VoiceSettings(const std::string& in_settings_file);

    // read/write from/to file etc...
    // web interface...
    // smooth transition to new settings...

    // eerst maar even public voor het gemak, later wellicht voor snelheid
    double attack;
    double decay;
    double sustain;
    double release;
};

#endif // VOICESETTINGS_H
