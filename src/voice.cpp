#include "voice.h"

#include <cmath>

Voice::Voice(std::shared_ptr<VoiceSettings> in_voice_settings,
             std::shared_ptr<VoiceGlobals> in_voice_globals, int in_number)
{
    voice_settings = in_voice_settings;
    voice_globals = in_voice_globals;
    number = in_number;

    wave = std::unique_ptr<Wave>(new Wave(voice_globals));
    main_adsr = std::unique_ptr<ADSR>(new ADSR(in_voice_globals->rate));

    buf.resize(voice_globals->bufsize, 0.0);
    active = false;
    note = 0;
}

void Voice::Start(int in_note)
{
    note = in_note;

    double halve_note = pow(2.0, 1.0/12.0);
    double frequency = 440.0 * pow(halve_note, (note-57));

    wave->Start(frequency, "noise");
    main_adsr->Start(voice_settings->attack,
                     voice_settings->decay,
                     voice_settings->sustain,
                     voice_settings->release);
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
        buf[i] = wave->Next() * main_adsr->Next(stop);
    }
    active = main_adsr->isActive();
}
