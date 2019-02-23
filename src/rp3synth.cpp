#include "rp3synth.h"

#include <iostream>  // TODO: moet weer weg
#include <valarray>

RP3Synth::RP3Synth(int in_n_voices,
                   int in_rate,
                   int in_bufsize,
                   const std::string& in_settings_file)
{
    std::cout << "RP3Synth constructor called" << std::endl;

    voicesettings = std::make_shared<VoiceSettings>(in_settings_file);
    voicegobals = std::make_shared<VoiceGlobals>(in_rate, in_bufsize);
    for (int i=0; i<in_n_voices; i++) {
        voices.emplace_back(std::make_shared<Voice>(voicesettings, voicegobals, i));
    }
    std::cout << "RP3Synth constructor end" << std::endl;
}

void RP3Synth::PlaybackCallback(short* buf)
{
    std::valarray<double> tmp_buf;
    tmp_buf.resize(voicegobals->bufsize, 0.0);

    for (std::shared_ptr<Voice> voice : voices) {
        if (voice->IsActive()) {
            voice->FillBuffer();
            tmp_buf += voice->buf;
        }
    }

    memset(buf, 0, voicegobals->bufsize * 4);
    int l1;
    for (l1 = 0; l1 < voicegobals->bufsize; l1++) {
        short sound;
        sound = (2^16/2) + 5000.0 * tmp_buf[l1];
        //sound = (2^16/2) + 5000.0 * (double)rand() / RAND_MAX;
        buf[2 * l1] += sound;
        buf[2 * l1 + 1] += sound;
    }
}

void RP3Synth::MidiCallback(snd_seq_event_t *ev)
{
    std::shared_ptr<Voice> voice;

    // fix for some keyboards, including mine, which do not send a note-off
    // event, but instead a note-on with zero velocity
    if ((ev->type == SND_SEQ_EVENT_NOTEON) && (ev->data.note.velocity == 0))
    {
        ev->type = SND_SEQ_EVENT_NOTEOFF;
    }

    switch (ev->type) {
        case SND_SEQ_EVENT_PITCHBEND:
            // change value to -1.0... to 1.0 (at least for my keyboard)
            voicegobals->pitch = ((ev->data.control.value-1) / 64) / 127.0;
            std::cout << "pitchbend : " << voicegobals->pitch << std::endl;
            break;
        case SND_SEQ_EVENT_CONTROLLER:
            std::cout << "modulator : " << ev->data.control.value << std::endl;
            break;
        case SND_SEQ_EVENT_NOTEON:
            voice = FindFreeVoice();
            if (voice)
            {
                voice->Start(ev->data.note.note);
            }
            break;
        case SND_SEQ_EVENT_NOTEOFF:
            for (auto voice : FindActiveVoices(ev->data.note.note)) {
                voice->Stop();
            }
            break;
    }
}

std::shared_ptr<Voice> RP3Synth::FindFreeVoice()
{
    for (auto voice : voices) {
        if (!voice->IsActive()) {
            return voice;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Voice>> RP3Synth::FindActiveVoices(int in_note)
{
    std::vector<std::shared_ptr<Voice>> retval;
    for (auto voice : voices) {
        if (voice->IsActive() && voice->GetNote() == in_note) {
            retval.push_back(voice);
        }
    }
    return retval;
}

void RP3Synth::Stop()
{
    voicesettings->Save();
}
