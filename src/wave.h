#ifndef WAVE_H
#define WAVE_H


class Wave
{
public:
    Wave(int in_rate);

    void Start(double in_frequency);
    double Next();

private:
    int rate;          // samples per seconds
    double phi;
    double phi_step;
};

#endif // WAVE_H
