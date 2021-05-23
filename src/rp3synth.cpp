#include "rp3synth.h"

#include <iostream>  // TODO: moet weer weg
#include <valarray>
#include <thread>
#include <math.h>

#include <malloc.h>

#include "utils.h"

/**
 * The constructor loads the settings file, sets up the voices and effects(chorus)
 *
 * @param in_n_voices       Number of polyphonics voices
 * @param in_rate           Output sampling rate in Hz
 * @param in_bufsize        Size of buffer, i.e. chuncks of audio
 * @param in_settings_file  Location of JSON settings file
 */

RP3Synth::RP3Synth(int in_n_voices,
                   int in_rate,
                   int in_bufsize,
                   const std::string& in_settings_file)
{
    std::cout << "RP3Synth constructor called" << std::endl;

    voicesettings = std::make_shared<VoiceSettings>(in_settings_file);
    voicegobals   = std::make_shared<VoiceGlobals>(in_rate, in_bufsize);

    for (int i=0; i<in_n_voices; i++) {
        voices.emplace_back(std::make_shared<Voice>(voicesettings, voicegobals));
    }

    rp3synth_buf.resize(voicegobals->bufsize, 0.0);

    chorus = std::make_unique<Chorus>(voicesettings, voicegobals);

    //logger = std::make_unique<Logger<short>>("log_rp3synth", std::size_t(1000000));

    std::cout << "RP3Synth constructor end" << std::endl;
}

/**
 * The destructors automatically cleans up all members and also writes the
 * settings file.
 */

RP3Synth::~RP3Synth()
{
    std::cout << "RP3Synth destructor" << std::endl;
}

/**
 * This callback is called by the soundsystem and fills a buffer with
 * a 'chunk' of PCM audio.
 *
 * @param buf The buffer to be filled
 */

void RP3Synth::PlaybackCallback(short* buf)
{
    // start threads for FillBuffer
    std::vector<std::shared_ptr<std::thread>> threads;
    std::vector<std::shared_ptr<Voice>> active_voices = FindActiveVoices();

    for (std::shared_ptr<Voice> voice : active_voices ) {
        threads.emplace_back(std::make_shared<std::thread>(std::thread(
          [](std::shared_ptr<Voice> v) {v->FillBuffer();} , voice
        )));
    }
    // wait for threads to finish
    for (std::shared_ptr<std::thread> th : threads) {
        th->join();
    }

    // collect voice signals
    rp3synth_buf = 0.0;
    for (std::shared_ptr<Voice> voice : active_voices) {
        rp3synth_buf += voice->buf;
    }

    // apply chorus
    chorus->Apply(&rp3synth_buf);

    // ALSA output, fill 'buf'
    for (std::size_t i=0;  i < voicegobals->bufsize; i++) {
        short sound =   10000.0
                      * voicesettings->master_volume
                      * rp3synth_buf[i];
        //sound = (32768-10) * (2/M_PI) * atan(voicesettings->master_volume * tmp_buf[i]);
        buf[2 * i]     =  sound;
        buf[2 * i + 1] = -sound; // de minus, dunno, klinkt luider, beter
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
        std::cout << "controller knob: "
                  << (int)ev->data.control.param
                  << " : "
                  << ev->data.control.value
                  << std::endl;
        voicesettings->HandleKnob(ev->data.control.param, ev->data.control.value);
        break;

    case SND_SEQ_EVENT_NOTEON:
        std::cout << "SND_SEQ_EVENT_NOTEON" << std::endl;
        // ff snel zo, m'n AKAI 'controller' is client 20
        if ((int)ev->source.client == 20) {
            std::cout << "controller button: "
                      << (int)ev->data.note.note
                      << std::endl;
            voicesettings->HandleButton(ev->data.note.note);
        } else {
            voice = FindFreeVoice();
            if (voice)
            {
                voice->Start(ev->data.note.note, ev->data.note.velocity);
            }
        }
        break;

    case SND_SEQ_EVENT_NOTEOFF:
        std::cout << "SND_SEQ_EVENT_NOTEOFF" << std::endl;
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
