#include "voiceglobals.h"

VoiceGlobals::VoiceGlobals(int in_rate, int in_bufsize)
{
    rate = in_rate;
    bufsize = in_bufsize;

    pitch = 0.0;
    modulation = 0.0;
}
