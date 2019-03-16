#include "chorus.h"

#include <cmath>

// short helper function, for (reasonable) negative array indices
inline int posmod(int i, int n) { return (i % n + n) % n; }


const double delay         = 0.05;   // seconds
const double depth         = 0.001;  // seconds
const double max_frequency = 10.0;   // Hz


Chorus::Chorus(std::shared_ptr<VoiceGlobals> in_voice_globals)
{
    voice_globals = in_voice_globals;
    rate          = voice_globals->rate;
    bufsize       = voice_globals->bufsize;

    // calculate history buffersize and start point
    int samples_needed   = (int)(rate  * (delay + 2 * depth)); // 1x depth 'safety' factor

    n_buffers    = (int)(ceil(samples_needed / bufsize));
    max_depth    = (int)(rate * depth);
    delay_offset = (int)(-rate * delay);

    history.resize(n_buffers * bufsize, 0.0);
    current_buffer = 0;
    phi = 0.0;
}


void Chorus::Apply(std::valarray<double>* buffer)
{
    // copy buffer to history
    current_buffer = (current_buffer + 1)  % n_buffers;
    int start = current_buffer * bufsize;
    history[std::slice(start, bufsize, 1)] = *buffer;

    // get parmeters from (my) keyboard knobs (modulators) (we don't store them yet)
    double gain      = voice_globals->modulation[21];
    double depth     = voice_globals->modulation[22];
    double frequency = voice_globals->modulation[23] * max_frequency;

    // create and fill chorused buffer
    std::valarray<double> chorused_buf(bufsize);
    double phi_step = 2.0 * M_PI * frequency / (double)rate;

    for (int i=0; i<bufsize; i++) {
        phi += phi_step;
        if (phi > 2.0 * M_PI) {
            phi -=  2 * M_PI;
        }
        // eerst nog even niet interpoleren
        int sine_index = (int)(depth* max_depth * sin(phi));
        int index = posmod(start + delay_offset + i + sine_index, n_buffers * bufsize);
        chorused_buf[i] = history[index];
    }

    // add chorus signal
    *buffer += gain * chorused_buf;
}
