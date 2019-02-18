#ifndef VOICE_H
#define VOICE_H

#include <memory>
#include <valarray>

#include "voicesettings.h"
#include "voiceglobals.h"
#include "wavesine.h"
#include "adsr.h"

class Voice
{
public:
    Voice(std::shared_ptr<VoiceSettings> in_voice_settings,
          std::shared_ptr<VoiceGlobals> in_voice_globals, int in_number);

    void Start(int in_note);
    void Stop();
    bool IsActive();
    int GetNote();
    void FillBuffer();

    std::valarray<double> buf;
    int number;

private:
    int note;
    bool active;
    bool stop;

    std::shared_ptr<VoiceSettings> voice_settings;
    std::shared_ptr<VoiceGlobals> voice_globals;

    std::unique_ptr<ADSR> main_adsr;
    std::unique_ptr<WaveSine> wave_sine;

};

#endif // VOICE_H
