#ifndef RP3SYNTH_H
#define RP3SYNTH_H

#include <memory>
#include <string>
#include <vector>
#include <alsa/asoundlib.h>

#include "voicesettings.h"
#include "voiceglobals.h"
#include "voice.h"

class RP3Synth
{
public:
    RP3Synth(int in_n_voices,
             int in_rate,
             int in_bufsise,
             const std::string& in_settings_file);

    void PlaybackCallback(short *buf);
    void MidiCallback(snd_seq_event_t *ev);
    void Stop();

private:

    std::shared_ptr<Voice> FindFreeVoice();
    std::shared_ptr<Voice> FindActiveVoice(int in_note);

    std::shared_ptr<VoiceSettings> voicesettings;
    std::shared_ptr<VoiceGlobals> voicegobals;
    std::vector<std::shared_ptr<Voice>> voices;
};

#endif // RP3SYNTH_H
