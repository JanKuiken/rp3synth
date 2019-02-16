#ifndef VOICEGLOBALS_H
#define VOICEGLOBALS_H


class VoiceGlobals
{
public:
    VoiceGlobals(int in_rate, int in_bufsize);

    // fixed for run
    int rate;
    int bufsize;

    // variables
    double pitch;       // pitchwheel value from MIDI keyboard
    double modulation;  // modwheel value from MIDI keyboard

};

#endif // VOICEGLOBALS_H
