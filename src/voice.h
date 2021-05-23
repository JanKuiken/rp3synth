#ifndef VOICE_H
#define VOICE_H

#include <memory>
#include <valarray>

#include "voicesettings.h"
#include "voiceglobals.h"
#include "wave.h"
#include "adsr.h"

class Voice
{
public:
    Voice(std::shared_ptr<VoiceSettings> in_voice_settings,
          std::shared_ptr<VoiceGlobals> in_voice_globals);
    ~Voice();

    void Start(int in_note, int in_velocity);
    void Stop();
    bool IsActive();
    int GetNote();
    void FillBuffer();

    std::valarray<double> buf;

private:
    int note;
    bool active;
    bool stop;

    std::shared_ptr<VoiceSettings> voice_settings;
    std::shared_ptr<VoiceGlobals> voice_globals;

    double velocity_factor;
    std::unique_ptr<ADSR> adsr_1;
    std::unique_ptr<ADSR> adsr_2;
    std::unique_ptr<Wave> wave_1;
    std::unique_ptr<Wave> wave_2;

};

#endif // VOICE_H
