#include "reverb.h"

#include <iostream>  // TODO: moet weer weg

const double MAX_DELAY     = 0.5;   // seconds

Reverb::Reverb(std::shared_ptr<VoiceGlobals> in_voice_globals)
{
    voice_globals = in_voice_globals;
    minimum_delay = (double)voice_globals->bufsize / (double)voice_globals->rate;
    delayed_signal = std::make_shared<DelayedSignal>(
         DelayedSignal(voice_globals->rate, MAX_DELAY * 2.0));
}

void Reverb::Apply(std::valarray<double>* buffer)
{
    // get parmeters from (my) keyboard knobs (modulators) (we don't store them yet)
    double gain      = voice_globals->modulation[25];
    double delay     = voice_globals->modulation[26] * MAX_DELAY;

    for (int i=0; i<voice_globals->bufsize; i++) {

        double current_value = (*buffer)[i];
        double delayed_value = delayed_signal->DelayedValue(delay);
        double new_value     = (current_value + gain * delayed_value) / (1.0 + gain);
//        std::cout << current_value << " "
//                  << delayed_value << " "
//                  << new_value << std::endl;
        (*buffer)[i] = new_value;
        //delayed_signal->AddValue(new_value);
        delayed_signal->AddValue(current_value);

    }
}

