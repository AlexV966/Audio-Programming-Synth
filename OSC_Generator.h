#ifndef Oscillators_h
#define Oscillators_h
#include <cmath>


class Oscillator
{
public:
    //define a public variable amplitude modulator to be used by all instance types
    float ampModulator = 1.0f;
    float sampleRate;
    double pi = 3.14159265358979323846264;
    
    //set sample rate
    void setSampleRate(float sampleRate_)
    {
        sampleRate = sampleRate_;
    }
    
  //set frequency
    void setFrequency(float inFreq)
    {
        frequency = inFreq;
        phaseDelta = frequency / sampleRate;
    }
    
    //add phase delta increment to the phase
    float process()
    {
        phase += phaseDelta;
        
        if (phase > 1.0f)
            phase -= 1.0f;
        
        return output(phase);
    }
    
    //declare a phase output that can be overriden
    virtual float output(float p)
    {
        return p;
    }
   
    
private:
    float frequency;
    float phase = 0.0f;
    float phaseDelta;
};

///All sub classes have an amplitude modulation option with the Square Osc class having a pulse width option and the Sine Osc having a power of and phase modulation option.

class SinOsc : public Oscillator
{
public:
    float output(float p) override
    {
        return outVal = powf((sin((p * 2.0f * pi) + phaseModulator)), powerOf) * ampModulator;
    }
    
    void setPowerOf()
    {
        TPO += 1;
        
        if(TPO >= (timePO * sampleRate))
        {
            TPO = 0;
            ranPower = (rand() % 10 + 1) * 1.0;
            powerOf = ranPower;
        }
        
    }
    
    void getPhaseModulator(float modIn2)
    {
        phaseModulator = modIn2;
    }
    
    void getAmpModulator(float modIn)
    {
        ampModulator = modIn;
    }
    
private:
    float powerOf = 1.0f;
    float ranPower;
    float phaseModulator = 0.0f;
    float outVal;
    int TPO = 0;
    float timePO = 35.0f;
};

class Phasor : public Oscillator
{
public:
    float output(float p) override
    {
        return p * ampModulator;
    }
    
    void getAmpModulator(float modIn)
    {
        ampModulator = modIn;
    }
};

class TriOsc : public Oscillator
{
public:
    float output(float p) override
    {
        return (fabsf(p - 0.5f) - 0.5f) * ampModulator;
    }
    
    void getAmpModulator(float modIn)
    {
        ampModulator = modIn;
    }
};

class SawOsc : public Oscillator
{
public:
    
    float output(float p) override
    {
        return outVal = ((p * 2) - 1.0f) * ampModulator;
    }
    
    void getAmpModulator(float modIn)
    {
        ampModulator = modIn;
    }
private:
    float outVal;
};


class SquareOsc : public Oscillator
{
public:
    float output(float p) override
    {
        float outVal = 0.5;
        if (p > pulseWidth)
        {
            outVal = -0.5;
        }
        
        return outVal * ampModulator;
    }
    
    void setPulseWidth(float pw)
    {
        pulseWidth = pw;
    }
    
    void getAmpModulator(float modIn)
    {
        ampModulator = modIn;
    }
private:
    float pulseWidth = 0.5f;
};




#endif 

