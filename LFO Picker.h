//
//  LFO Picker.h
//  JUCE SYNTH
//
//  Created by Alexandru Vasile on 21/12/2022.
//

#ifndef LFO_Picker_h
#define LFO_Picker_h
#include "LFO_Generator.h"

class LFO_Picker
{
public:
    void setSampleRate(float _sampleRate)
    {
        sampleRate = _sampleRate;
        
        sinLFO.setSampleRate(sampleRate);
        triLFO.setSampleRate(sampleRate);
        squareLFO.setSampleRate(sampleRate);
        sawLFO.setSampleRate(sampleRate);
    }
    
    void setFrequency(float _freq)
    {
        
        freq = _freq;
        
        sinLFO.setFrequency(freq);
        triLFO.setFrequency(freq);
        squareLFO.setFrequency(freq);
        sawLFO.setFrequency(freq);
    }
    
    void setDepthAndOffset(float _depth, float _offset)
    {
        depth = _depth;
        offset = _offset;
        
        sinLFO.setDepth(depth);
        triLFO.setDepth(depth);
        squareLFO.setDepth(depth);
        sawLFO.setDepth(depth);
        
        sinLFO.setOffset(offset);
        triLFO.setOffset(offset);
        squareLFO.setOffset(offset);
        sawLFO.setOffset(offset);
        
    }
    
    float process(int valueIn)
    {
        if(valueIn == 0)
        {
            output = 1;
        }
        
        if(valueIn == 1)
        {
            output = sinLFO.process();
        }
        
        if(valueIn == 2)
        {
            output = triLFO.process();
        }
        
        if(valueIn == 3)
        {
            output = sawLFO.process();
        }
        
        if(valueIn == 4)
        {
            output = squareLFO.process();
        }
        
        
        return output;
    }
    
    
    
    
    
private:
    SinLFO sinLFO;
    TriLFO triLFO;
    SquareLFO squareLFO;
    SawLFO sawLFO;
    float sampleRate;
    float freq;
    float depth;
    float offset;
    float pulseWidth;
    float output;
};

#endif

