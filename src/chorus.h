#ifndef CHORUS_H
#define CHORUS_H

#include <memory>
#include <valarray>

#include "voiceglobals.h"

class Chorus
{
public:
    Chorus(std::shared_ptr<VoiceGlobals> in_voice_globals);

    void Apply(std::valarray<double>* buffer);

private:
    std::shared_ptr<VoiceGlobals> voice_globals;
    std::valarray<double> history;  // ringbuffer of the incoming signal
    int rate;                       // samples per seconds
    int bufsize;
    int n_buffers;
    int current_buffer;
    int delay_offset;               // start point for chorus in samples
    int max_depth;                  // max chorus depth in samples
    double phi;

};

#endif // CHORUS_H
