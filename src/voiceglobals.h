#ifndef VOICEGLOBALS_H
#define VOICEGLOBALS_H

#include <cstddef>

// const int n_modulations = 128;

class VoiceGlobals
{
public:
    VoiceGlobals(std::size_t in_rate, std::size_t in_bufsize);

    // fixed for run
    std::size_t rate;
    std::size_t bufsize;

    // variables
    double pitch;                      // pitchwheel value from MIDI keyboard
    //double modulation[n_modulations];  // modulation values from MIDI keyboard

    //void SetModulation(unsigned int param, int value);

};

#endif // VOICEGLOBALS_H
