#ifndef RP3SYNTH_H
#define RP3SYNTH_H

#include <memory>
#include <string>
#include <vector>
#include <alsa/asoundlib.h>

#include "voicesettings.h"
#include "voiceglobals.h"
#include "chorus.h"
#include "voice.h"

#include "logger.h"

class RP3Synth
{
public:
    RP3Synth(int in_n_voices,
             int in_rate,
             int in_bufsise,
             const std::string& in_settings_file);
    ~RP3Synth();

    void PlaybackCallback(short *buf);
    void MidiCallback(snd_seq_event_t *ev);

private:

    std::valarray<double> rp3synth_buf; /// Buffer for the audio chunks

    std::shared_ptr<Voice> FindFreeVoice();
    std::vector<std::shared_ptr<Voice>> FindActiveVoices();
    std::vector<std::shared_ptr<Voice>> FindActiveVoices(int in_note);

    std::shared_ptr<VoiceSettings> voicesettings;
    std::shared_ptr<VoiceGlobals> voicegobals;
    std::vector<std::shared_ptr<Voice>> voices;
    std::unique_ptr<Chorus> chorus;

    std::unique_ptr<Logger<short>> logger;
};

#endif // RP3SYNTH_H
