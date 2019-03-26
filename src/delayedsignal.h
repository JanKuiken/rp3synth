#ifndef DELAYEDSIGNAL_H
#define DELAYEDSIGNAL_H

#include <valarray>

/*
 * Implements a ringbuffer
 *  - input are chunks of data (const std::valarray<double>)
 *  - size of incoming data must be bufsize
 */

class DelayedSignal
{
public:
    DelayedSignal(int in_rate, int in_bufsize, double in_max_delay);
    void AddBuffer(const std::valarray<double>* buffer);
    void UpdateLastBuffer(const std::valarray<double>* buffer);
    double DelayedValue(int in_delay_samples);
    double DelayedValue(double in_delay_seconds);
    void AdvanceIndex();

private:
    std::valarray<double> history;  // ringbuffer of the incoming signal
    int rate;                       // samples per seconds
    int bufsize;
    int n_buffers;
    int size;                       // sample size of 'history' in samples
    int current_buffer;
    int current_index;
};

#endif // DELAYEDSIGNAL_H
