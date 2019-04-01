#include "chorus.h"

#include <cmath>

const double DELAY         = 0.05;   // seconds
const double MAX_DEPTH     = 0.001;  // seconds
const double MAX_FREQUENCY = 10.0;   // Hz

Chorus::Chorus(std::shared_ptr<VoiceGlobals> in_voice_globals)
{
    voice_globals = in_voice_globals;
    delayed_signal = std::make_shared<DelayedSignal>(
         DelayedSignal(voice_globals->rate, DELAY + 2 * MAX_DEPTH)
    );
    phi  = 0.0;
}


void Chorus::Apply(std::valarray<double>* buffer)
{
    // get parmeters from (my) keyboard knobs (modulators) (we don't store them yet)
    double gain      = voice_globals->modulation[21];
    double depth     = voice_globals->modulation[22];
    double frequency = voice_globals->modulation[23] * MAX_FREQUENCY;

    double phi_step = 2.0 * M_PI * frequency / (double)voice_globals->rate;

    for (int i=0; i<voice_globals->bufsize; i++) {

        phi += phi_step;
        if (phi > 2.0 * M_PI) {
            phi -=  2 * M_PI;
        }

        delayed_signal->AddValue((*buffer)[i]);
        double delay_seconds = DELAY + MAX_DEPTH * depth * sin(phi);
        double chorused_value = delayed_signal->DelayedValue(delay_seconds);
        (*buffer)[i] += gain * chorused_value / (1.0 + gain);

    }
}
