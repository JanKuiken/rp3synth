#ifndef REVERB_H
#define REVERB_H

#include <memory>
#include <valarray>

#include "voiceglobals.h"
#include "delayedsignal.h"

class Reverb
{
public:
    Reverb(std::shared_ptr<VoiceGlobals> in_voice_globals);

    void Apply(std::valarray<double>* buffer);

private:
    std::shared_ptr<VoiceGlobals> voice_globals;
    std::shared_ptr<DelayedSignal> delayed_signal;

    double minimum_delay;
};

#endif // REVERB_H
