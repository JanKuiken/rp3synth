#ifndef VOICEGLOBALS_H
#define VOICEGLOBALS_H

const int n_modulations = 128;

class VoiceGlobals
{
public:
    VoiceGlobals(int in_rate, int in_bufsize);

    // fixed for run
    int rate;
    int bufsize;

    // variables
    double pitch;                      // pitchwheel value from MIDI keyboard
    double modulation[n_modulations];  // modulation values from MIDI keyboard

    void SetModulation(unsigned int param, int value);

};

#endif // VOICEGLOBALS_H
