#ifndef ADSR_H
#define ADSR_H

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

    void Start(double in_attack,
               double in_decay,
               double in_sustain,
               double in_release);
    double Next(bool in_stop);    // returns next value and maintains state
    bool isActive();

private:
    int rate;          // samples per seconds
    State state;
    bool stopped;
    double attack;     // attack time [seconds]
    double decay;      // decay time [seconds]
    double sustain;    // sustain level [0.0 .. 1.0]
    double release;    // release time [seconds]

    int counter;       // ticks
    int attack_ticks;  // ticks
    int decay_ticks;   // ticks
    int release_ticks; // ticks
};

#endif // ADSR_H
