//
//  Vector_Harmonics_Generators.h
//
//
//
//

#ifndef VectorOSCHarmonics_h
#define VectorOSCHarmonics_h
#include <vector>
#include "OSC_Generator.h"

class VectorHarmonics
{
public:
    
    //setting the sample rate
    void setSampleRate(float sampleRate_)
    {
        sampleRate = sampleRate_;
    }
    
    //setting the fundamental
    void setFundamental(float fundamental_)
    {
        fundamentalFreq = fundamental_;
    }
    
    //setting the desired number of harmonics
    void setOscCount(float oscCount_)
    {
        harmonicsCount = oscCount_;
    }
    
    //clearing the vector
    void clear()
    {
        oscVector.clear();
    }
    
    //populating the vector
    void setHarmonicsCount()
    {
        // add oscillators to the vector and set samplerate and frequencies
        for (int i=0; i< harmonicsCount; i++)
        {
            // add an oscillator to the vector (increasing the vector size by 1)
            oscVector.push_back(SinOsc());
            
            // set the sample rate of the newly added SinOsc
            oscVector[i].setSampleRate(sampleRate);
            
            // make frequency a per harmonic thing
            float harmonics = fundamentalFreq * (i + 1);
            oscVector[i].setFrequency(harmonics);
        }
        
    }
    
    float process()
    {
            // create variable to store the combined output of all oscillators
            float additiveOutput = 0;
            
            // loop through all the oscillators in the vector
            for (int j=0; j<oscVector.size(); j++)
            {
                // sum up the oscillator outputs into one output mix
                additiveOutput += oscVector[j].process() / float(harmonicsCount);
            }
        
         // return the output mix
        return additiveOutput;
    }
   
    
private:
    std::vector<SinOsc> oscVector;
    float sampleRate;
    float fundamentalFreq;
    float harmonicsCount;
    
};

#endif 

