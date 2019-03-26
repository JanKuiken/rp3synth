#include "delayedsignal.h"

#include <iostream>  // TODO: moet weer weg
#include <cmath>

// short helper function.
inline int posmod(int i, int n) { return (i % n + n) % n; }


DelayedSignal::DelayedSignal(int in_rate, int in_bufsize, double in_max_delay)
{
    rate               = in_rate;
    bufsize            = in_bufsize;

    int samples_needed = (int)(rate  * in_max_delay);
    n_buffers          = (int)(ceil((double)samples_needed / (double)bufsize));
    size               = n_buffers * bufsize;
    current_buffer     = -1;
    current_index      = -1;

    history.resize(size, 0.0);
    std::cout << " n_buffers : "  << n_buffers
              << " size : "       << size
              << " rate : "       << rate
              << std::endl;
}

void DelayedSignal::AddBuffer(const std::valarray<double>* buffer)
{
    current_buffer = (current_buffer + 1)  % n_buffers;
    int start      = current_buffer * bufsize;
    current_index  = start + bufsize - 1;

    history[std::slice(start, bufsize, 1)] = *buffer;
}

void DelayedSignal::UpdateLastBuffer(const std::valarray<double>* buffer)
{
    int start      = current_buffer * bufsize;
    history[std::slice(start, bufsize, 1)] = *buffer;
}

void DelayedSignal::AdvanceIndex()
{
    current_index += 1;
}

double DelayedSignal::DelayedValue(int in_delay_samples)
{
    int index = posmod(current_index - in_delay_samples, size);
    return history[index];
}

double DelayedSignal::DelayedValue(double in_delay_seconds)
{
    // eerst nog even niet interpoleren
    int delay_samples = (int)(in_delay_seconds * rate);
    int index = posmod(current_index - delay_samples, size);
    return   history[index];
}

