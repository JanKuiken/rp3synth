#include "delayedsignal.h"

#include <iostream>  // TODO: moet weer weg
#include <cmath>

// short helper function.
inline int posmod(int i, int n) { return (i % n + n) % n; }


DelayedSignal::DelayedSignal(int in_rate, double in_max_delay)
{
    rate = in_rate;
    size = (int)ceil(rate  * in_max_delay);
    history.resize(size, 0.0);
    current_index = 0;
}

void DelayedSignal::AddValue(double value)
{
    current_index = posmod(current_index + 1, size);
    history[current_index] = value;
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

