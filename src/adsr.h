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
    bool isActive();
    void Start(double in_attack,
               double in_decay,
               double in_sustain,
               double in_release);
    double Next(bool in_stop);    // returns next value and maintains state

private:

    int attack_ticks;  // ticks
    int decay_ticks;   // ticks
    double sustain;    // sustain level [0.0 .. 1.0]
    int release_ticks; // ticks

    int rate;          // samples per seconds

    State state;
    bool stopped;
    int counter;       // ticks
};

#endif // ADSR_H
