#include "voicesettings.h"

#include <iostream>  // TODO: moet weer weg

VoiceSettings::VoiceSettings(const std::string& in_settings_file)
{
    std::cout << "NOT reading file : " << in_settings_file << std::endl;

    attack  = 0.05;
    decay   = 0.05;
    sustain = 0.9;
    release = 0.05;
}
