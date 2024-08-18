//Distorsion_Class.h

#ifndef Distorsion_h
#define Distorsion_h
#include <cmath>

class Distortion
{
public:
    
    float distortionClipper(float inSample, float inLFO_)
    {
        //converting the input of a straight sine into the range 1. to 10.
        gain = (inLFO_ + 1.0) * 4.5 + 1.0;
        float output = tanh(inSample * gain);
        return output;
    }
private:
    float gain;
};

#endif
