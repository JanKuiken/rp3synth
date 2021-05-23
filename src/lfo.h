#ifndef LFO_H
#define LFO_H

#include <memory>

#include "voicesettings.h"
#include "voiceglobals.h"
#include "wave.h"

class LFO
{
public:
    LFO(std::shared_ptr<VoiceSettings> in_voice_settings,
        std::shared_ptr<VoiceGlobals> in_voice_globals, int in_number);
    ~LFO();

    double Next();

private:
    std::unique_ptr<Wave> wave;

};

#endif // LFO_H

