#ifndef WAVESINE_H
#define WAVESINE_H


class WaveSine
{
public:
    WaveSine(int in_rate);

    void Start(double in_frequency);
    double Next();

private:
    int rate;          // samples per seconds
    double phi;
    double phi_step;
};

#endif // WAVESINE_H
