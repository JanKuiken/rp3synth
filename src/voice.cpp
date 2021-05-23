#include "voice.h"

#include <cmath>
#include <iostream>  // TODO: moet weer weg

#include "utils.h"

Voice::Voice(std::shared_ptr<VoiceSettings> in_voice_settings,
             std::shared_ptr<VoiceGlobals> in_voice_globals)
{
    //std::cout << "Voice constructor called" << std::endl;

    voice_settings = in_voice_settings;
    voice_globals = in_voice_globals;

    wave_1 = std::make_unique<Wave>(voice_globals);
    wave_2 = std::make_unique<Wave>(voice_globals);

    adsr_1 = std::make_unique<ADSR>(in_voice_globals->rate);
    adsr_2 = std::make_unique<ADSR>(in_voice_globals->rate);

    buf.resize(voice_globals->bufsize, 0.0);
    active = false;
    note = 0;
}

Voice::~Voice()
{
    //std::cout << "Voice destructor called" << std::endl;
}

void Voice::Start(int in_note, int in_velocity)
{
    std::cout << "Voice::Start" << std::endl;

    note = in_note;

    const double halve_note = pow(2.0, 1.0/12.0);
    double frequency = 0.25 * 440.0 * pow(halve_note, (note-57));
    double velocity = bounds_limit(in_velocity / 127.0, 0.0, 1.0);
    velocity_factor = sqrt(velocity);

    wave_1->Start(frequency,
                  voice_settings->detune_1,
                  voice_settings->waveform_1);
    adsr_1->Start(voice_settings->attack_1,
                  voice_settings->decay_1,
                  voice_settings->sustain_1,
                  voice_settings->release_1);

    wave_2->Start(frequency,
                  voice_settings->detune_2,
                  voice_settings->waveform_2);
    adsr_2->Start(voice_settings->attack_2,
                  voice_settings->decay_2,
                  voice_settings->sustain_2,
                  voice_settings->release_2);
    active = true;
    stop = false;
}

void Voice::Stop() {
    std::cout << "Voice::Stop" << std::endl;
    stop = true;
}

bool Voice::IsActive()
{
    return active;
}

int Voice::GetNote()
{
    return note;
}

void Voice::FillBuffer()
{
    buf = 0.0;
    for (std::size_t i=0; i<voice_globals->bufsize; i++) {

        // TODO: stop gedoe uit zoeken....
        double w1 = wave_1->Next(stop); // stop wave after adsr
        double w2 = wave_2->Next(stop);
        double a1 = adsr_1->Next(stop);
        double a2 = adsr_2->Next(stop);

        buf[i] =  w1 * a1 + w2 * a2;
        buf[i] = velocity_factor *
            (  wave_1->Next(stop) * adsr_1->Next(stop)
             + wave_2->Next(stop) * adsr_2->Next(stop) );
    }
    active = adsr_1->isActive() || adsr_2->isActive();
}
