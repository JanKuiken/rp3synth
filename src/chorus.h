#ifndef CHORUS_H
#define CHORUS_H

#include <memory>
#include <valarray>

#include "voiceglobals.h"
#include "delayedsignal.h"

class Chorus
{
public:
    Chorus(std::shared_ptr<VoiceGlobals> in_voice_globals);

    void Apply(std::valarray<double>* buffer);

private:
    std::shared_ptr<VoiceGlobals> voice_globals;
    std::shared_ptr<DelayedSignal> delayed_signal;

    double phi;
};

#endif // CHORUS_H
