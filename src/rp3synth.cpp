#include "rp3synth.h"

#include <alsa/asoundlib.h>

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
            //std::shared_ptr<std::valarray<double>> vbuf = voice->GetBuffer();
            voice->FillBuffer();
            //std::cout << "count " << voice->buf.size() << std::endl;
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
    //return;
    //std::cout << "RP3Synth::MidiCallback 0x" << std::hex << ev->data.note.note << std::endl;
    std::shared_ptr<Voice> voice;

    // fix for some keyboards, including mine, which do not send a note-off
    // event, but instead a note-on with zero velocity
    if ((ev->type == SND_SEQ_EVENT_NOTEON) && (ev->data.note.velocity == 0))
    {
        ev->type = SND_SEQ_EVENT_NOTEOFF;
    }

    switch (ev->type) {
        case SND_SEQ_EVENT_PITCHBEND:
            //pitch = (double)ev->data.control.value / 8192.0;
            //std::cout << "pitchbend : " << ev->data.control.value << std::endl;
            break;
        case SND_SEQ_EVENT_CONTROLLER:
            //if (ev->data.control.param == 1) {
            //    modulation = (double)ev->data.control.value / 10.0;
            //}
            std::cout << "modulator : " << ev->data.control.value << std::endl;
            break;
        case SND_SEQ_EVENT_NOTEON:
            voice = FindFreeVoice();
            if (voice)
            {
                //std::cout << "Start event" << std::endl;
                voice->Start(ev->data.note.note);
                //std::cout << "Start event2" << std::endl;
            }
            break;
        case SND_SEQ_EVENT_NOTEOFF:
            // stop all voices with this note
            //bool cont = true;
            //while (cont)
            //{
                voice = FindActiveVoice(ev->data.note.note);
                if (voice)
                {
                    //std::cout << "Stop event" << std::endl;
                    voice->Stop();
                }
            //    else
            //    {
            //        cont = false;
            //    }
            //}
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

std::shared_ptr<Voice> RP3Synth::FindActiveVoice(int in_note)
{

    for (auto voice : voices) {
        if (voice->IsActive() && voice->GetNote() == in_note) {
            return voice;
        }
    }
    return nullptr;
}

