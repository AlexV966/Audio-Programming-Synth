//
//  LFO_Generator.h
//
//
//
//

#ifndef LFO_Class_h
#define LFO_Class_h
#include <cmath>

class LFO
{
public:
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
   
    //set a phase delta stretch
    void setPhaseDeltaStretch(float inValue)
    {
        phaseDeltaStretch = inValue;
        
        if(phaseDeltaStretch < 1.0f)
        {
            phaseDeltaStretch = 1.0f;
        }
        
        if(phaseDeltaStretch > 10.0f)
        {
            phaseDeltaStretch = 10.0f;
        }
    }
    
    //add phase delta increment to the phase
    float process()
    {
        phase += (phaseDelta / phaseDeltaStretch);
        
        if (phase > 1.0f)
            phase -= 1.0f;
        
        return output(phase);
    }
    
    //subtract the increment to help in creating a reversed ramp
    float reverseProcess()
    {
        phase -= (phaseDelta / phaseDeltaStretch);
       
        if (phase <= 0.0f)
            phase += 1.0f;
        
        return revRampOutput(phase);
        
    }
    
    //declare a phase output that can be overriden
    virtual float output(float p)
    {
        return p;
    }
    
    //declare a reverse phase output that can be overriden
    virtual float revRampOutput(float rp)
    {
        return rp;
    }
    
    double pi = 3.14159265358979323846264;
    float offset = 0.0f;
    float depth = 1.0f;
private:
    float frequency;
    float sampleRate;
    float phase = 0.0f;
    float phaseDelta;
    float phaseDeltaStretch = 1.0f;
};

///All subclasses have been fitted with a depth and offset option

class RampLFO : public LFO
{
public:
    float output(float p)
    {
        return ((p * depth) + offset);
    }
   
    //set an offset for the lfo's
    void setOffset(float inOffset)
    {
        offset = inOffset;
        
        if(offset < 0.0f)
        {
            offset = 0.0f;
        }
        
        if(offset > 1.0f)
        {
            offset = 1.0f;
        }
    }
    
    //set an lfo depth
    void setDepth(float inDepth)
    {
        depth = inDepth;
        
        if(depth < 0.0f)
        {
            depth = 0.0f;
        }
        
        if(depth > 1.0f)
        {
            depth = 1.0f;
        }
    }
};

class revRampLFO : public LFO
{
public:
    
    float revRampOutput(float rp)
    {
        return ((rp * depth) + offset);
    }
    
    //set an offset for the lfo's
    void setOffset(float inOffset)
    {
        offset = inOffset;
        
        if(offset < 0.0f)
        {
            offset = 0.0f;
        }
        
        if(offset > 1.0f)
        {
            offset = 1.0f;
        }
    }
    
    //set an lfo depth
    void setDepth(float inDepth)
    {
        depth = inDepth;
        
        if(depth < 0.0f)
        {
            depth = 0.0f;
        }
        
        if(depth > 1.0f)
        {
            depth = 1.0f;
        }
    }
};

class SinLFO : public LFO
{
public:
    float output(float p) override
    {
        return outVal = (sin(p * 2.0f * pi)) * depth + offset;
    }
    
    //set an offset for the lfo's
    void setOffset(float inOffset)
    {
        offset = inOffset;
        
        if(offset < 0.0f)
        {
            offset = 0.0f;
        }
        
        if(offset > 1.0f)
        {
            offset = 1.0f;
        }
    }
    
    //set an lfo depth
    void setDepth(float inDepth)
    {
        depth = inDepth;
        
        if(depth < 0.0f)
        {
            depth = 0.0f;
        }
        
        if(depth > 1.0f)
        {
            depth = 1.0f;
        }
    }
private:
    float outVal;
};

class TriLFO : public LFO
{
public:
    float output(float p) override
    {
        return (fabsf(p - 0.5f) - 0.5f) * depth + offset;
    }
    
    //set an offset for the lfo's
    void setOffset(float inOffset)
    {
        offset = inOffset;
        
        if(offset < 0.0f)
        {
            offset = 0.0f;
        }
        
        if(offset > 1.0f)
        {
            offset = 1.0f;
        }
    }
    
    //set an lfo depth
    void setDepth(float inDepth)
    {
        depth = inDepth;
        
        if(depth < 0.0f)
        {
            depth = 0.0f;
        }
        
        if(depth > 1.0f)
        {
            depth = 1.0f;
        }
    }
};

class SawLFO : public LFO
{
public:
    float output(float p) override
    {
        return outVal = (((p * 2) - 1.0f)) * depth + offset;
    }
    
    //set an offset for the lfo's
    void setOffset(float inOffset)
    {
        offset = inOffset;
        
        if(offset < 0.0f)
        {
            offset = 0.0f;
        }
        
        if(offset > 1.0f)
        {
            offset = 1.0f;
        }
    }
    
    //set an lfo depth
    void setDepth(float inDepth)
    {
        depth = inDepth;
        
        if(depth < 0.0f)
        {
            depth = 0.0f;
        }
        
        if(depth > 1.0f)
        {
            depth = 1.0f;
        }
    }
private:
    float outVal;
};


class SquareLFO : public LFO
{
public:
    float output(float p) override
    {
        float outVal = 0.5;
        if (p > pulseWidth)
            outVal = -0.5;
        return outVal * depth + offset;
    }
    
    void setPulseWidth(float pw)
    {
        pulseWidth = pw;
    }
    
    //set an offset for the lfo's
    void setOffset(float inOffset)
    {
        offset = inOffset;
        
        if(offset < 0.0f)
        {
            offset = 0.0f;
        }
        
        if(offset > 1.0f)
        {
            offset = 1.0f;
        }
    }
    
    //set an lfo depth
    void setDepth(float inDepth)
    {
        depth = inDepth;
        
        if(depth < 0.0f)
        {
            depth = 0.0f;
        }
        
        if(depth > 1.0f)
        {
            depth = 1.0f;
        }
    }
    
private:
    float pulseWidth = 0.5f;
};


#endif 

