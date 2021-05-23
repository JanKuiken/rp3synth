#ifndef ADSR_H
#define ADSR_H

#include <memory>

#include "logger.h"

enum State { idle,
             key_attack,
             key_decay,
             key_sustain,
             key_release
           };

class ADSR
{
public:
    ADSR(int in_rate);
    bool isActive();
    void Start(double in_attack,
               double in_decay,
               double in_sustain,
               double in_release);
    double Next(bool in_stop);    // returns next value and maintains state

    ~ADSR();

private:

    int rate;          // samples per seconds

    double attack_rate;
    double decay_rate;
    double sustain_level;
    double release_rate;


    State state;
    bool stopped;
    double current_val;

    std::unique_ptr<Logger<double>> logger;
};

#endif // ADSR_H
