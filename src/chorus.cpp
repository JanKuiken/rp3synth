#include "chorus.h"

#include <cmath>

const double DELAY         = 0.05;   // seconds
const double MAX_DEPTH     = 0.001;  // seconds
const double MAX_FREQUENCY = 10.0;   // Hz

Chorus::Chorus(std::shared_ptr<VoiceGlobals> in_voice_globals)
{
    voice_globals = in_voice_globals;
    delayed_signal = std::make_shared<DelayedSignal>(
         DelayedSignal(voice_globals->rate,
                       voice_globals->bufsize,
                       DELAY + 2 * MAX_DEPTH
         )
    );
    phi  = 0.0;
}


void Chorus::Apply(std::valarray<double>* buffer)
{
    // copy buffer to history
    delayed_signal->AddBuffer(buffer);

    // get parmeters from (my) keyboard knobs (modulators) (we don't store them yet)
    double gain      = voice_globals->modulation[21];
    double depth     = voice_globals->modulation[22];
    double frequency = voice_globals->modulation[23] * MAX_FREQUENCY;

    // create and fill chorused buffer
    std::valarray<double> chorused_buf(voice_globals->bufsize);
    double phi_step = 2.0 * M_PI * frequency / (double)voice_globals->rate;

    for (int i=0; i<voice_globals->bufsize; i++) {
        phi += phi_step;
        if (phi > 2.0 * M_PI) {
            phi -=  2 * M_PI;
        }
        double delay_seconds = DELAY + MAX_DEPTH * depth * sin(phi);
        chorused_buf[i] = delayed_signal->DelayedValue(delay_seconds);

        delayed_signal->AdvanceIndex();
    }

    // add chorus signal
    *buffer += gain * chorused_buf / (1.0 + gain);
}
