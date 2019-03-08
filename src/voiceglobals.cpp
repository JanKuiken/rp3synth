#include "voiceglobals.h"

#include "utils.h"

VoiceGlobals::VoiceGlobals(int in_rate, int in_bufsize)
{
    rate = in_rate;
    bufsize = in_bufsize;

    pitch = 0.0;
    for (int i=0; i<n_modulations; i++) modulation[i] = 0.0;
}

void VoiceGlobals::SetModulation(unsigned int param, int value)
{
    if (param >= 0 and param < n_modulations) {
        modulation[param] = bounds_limit(value / 127.0, 0.0, 1.0);
    }
}
