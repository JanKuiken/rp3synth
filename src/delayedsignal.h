#ifndef DELAYEDSIGNAL_H
#define DELAYEDSIGNAL_H

#include <valarray>


class DelayedSignal
{
public:
    DelayedSignal(int in_rate, double in_max_delay);
    double DelayedValue(int in_delay_samples);
    double DelayedValue(double in_delay_seconds);
    void AddValue(double value);

private:
    std::valarray<double> history;  // ringbuffer of the incoming signal
    int rate;                       // samples per seconds
    int size;                       // sample size of 'history' in samples
    int current_index;
};

#endif // DELAYEDSIGNAL_H
