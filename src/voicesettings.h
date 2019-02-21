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
    double attack;
    double decay;
    double sustain;
    double release;

private:
    std::string settings_file;
};

#endif // VOICESETTINGS_H
