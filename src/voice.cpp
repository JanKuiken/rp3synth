#include "voice.h"

#include <cmath>

#include "utils.h"

Voice::Voice(std::shared_ptr<VoiceSettings> in_voice_settings,
             std::shared_ptr<VoiceGlobals> in_voice_globals, int in_number)
{
    voice_settings = in_voice_settings;
    voice_globals = in_voice_globals;
    number = in_number;

    wave_1 = std::unique_ptr<Wave>(new Wave(voice_globals));
    adsr_1 = std::unique_ptr<ADSR>(new ADSR(in_voice_globals->rate));

    wave_2 = std::unique_ptr<Wave>(new Wave(voice_globals));
    adsr_2 = std::unique_ptr<ADSR>(new ADSR(in_voice_globals->rate));

    buf.resize(voice_globals->bufsize, 0.0);
    active = false;
    note = 0;
}

void Voice::Start(int in_note, int in_velocity)
{
    note = in_note;

    double halve_note = pow(2.0, 1.0/12.0);
    double frequency = 440.0 * pow(halve_note, (note-57));
    double velocity = bounds_limit(in_velocity / 127.0, 0.0, 1.0);
    velocity_factor = sqrt(velocity);

    wave_1->Start(frequency, "square");
    adsr_1->Start(voice_settings->attack,
                  voice_settings->decay,
                  voice_settings->sustain,
                  voice_settings->release);

    wave_2->Start(frequency, "noise");
    adsr_2->Start(voice_settings->attack * 2.0,
                  voice_settings->decay * 0.5,
                  voice_settings->sustain * 0.4,
                  0.0);
    active = true;
    stop = false;
}

void Voice::Stop() {
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
    for (int i=0; i<voice_globals->bufsize; i++) {
        buf[i] = velocity_factor *  (  wave_1->Next() * adsr_1->Next(stop)
                                     + wave_2->Next() * adsr_2->Next(stop) );
    }
    active = adsr_1->isActive();
}
