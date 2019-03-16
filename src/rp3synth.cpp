#include "rp3synth.h"

#include <iostream>  // TODO: moet weer weg
#include <valarray>
#include <thread>

#include "utils.h"

RP3Synth::RP3Synth(int in_n_voices,
                   int in_rate,
                   int in_bufsize,
                   const std::string& in_settings_file)
{
    std::cout << "RP3Synth constructor called" << std::endl;

    voicesettings = std::make_shared<VoiceSettings>(in_settings_file);
    voicegobals = std::make_shared<VoiceGlobals>(in_rate, in_bufsize);
    for (int i=0; i<in_n_voices; i++) {
        voices.emplace_back(std::make_shared<Voice>(voicesettings, voicegobals));
    }
    chorus = std::make_unique<Chorus>(voicegobals);
    std::cout << "RP3Synth constructor end" << std::endl;
}

void RP3Synth::PlaybackCallback(short* buf)
{
    std::valarray<double> tmp_buf;
    tmp_buf.resize(voicegobals->bufsize, 0.0);

    // start threads for FillBuffer
    std::vector<std::thread> threads;
    std::vector<std::shared_ptr<Voice>> active_voices = FindActiveVoices();
    for (std::shared_ptr<Voice> voice : active_voices ) {
        threads.emplace_back(std::thread(
            [](std::shared_ptr<Voice> v) {v->FillBuffer();} , voice
        ));
    }
    // wait for threads to finish
    for (std::thread& th : threads) {
        th.join();
    }
    // collect voice signals
    for (std::shared_ptr<Voice> voice : active_voices) {
        tmp_buf += voice->buf;
    }

    // apply chorus
    chorus->Apply(&tmp_buf);

    // ALSA output
    memset(buf, 0, voicegobals->bufsize * 4);
    for (int i=0;  i < voicegobals->bufsize; i++) {
        short sound;
        sound = (2^16/2) + 5000.0 * tmp_buf[i];
        buf[2 * i] += sound;
        buf[2 * i + 1] += sound;
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
            voicegobals->pitch = bounds_limit((ev->data.control.value-1) / (64 * 127.0), -1.0, 1.0);
            break;
        case SND_SEQ_EVENT_CONTROLLER:
            voicegobals->SetModulation(ev->data.control.param, ev->data.control.value);
            break;
        case SND_SEQ_EVENT_NOTEON:
            voice = FindFreeVoice();
            if (voice)
            {
                voice->Start(ev->data.note.note, ev->data.note.velocity);
            }
            break;
        case SND_SEQ_EVENT_NOTEOFF:
            for (auto voice : FindActiveVoices(ev->data.note.note)) {
                voice->Stop();
            }
            break;
        case SND_SEQ_EVENT_KEYPRESS:
            std::cout << "aftertouch : " << ev->data.control.value << std::endl;
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

std::vector<std::shared_ptr<Voice>> RP3Synth::FindActiveVoices()
{
    std::vector<std::shared_ptr<Voice>> retval;
    for (auto voice : voices) {
        if (voice->IsActive()) {
            retval.push_back(voice);
        }
    }
    return retval;
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
