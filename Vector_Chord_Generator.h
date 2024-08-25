//
//  Vector_Harmonics_Generators.h
//
//
//
//

#ifndef VectorOSCHarmonics_h
#define VectorOSCHarmonics_h
#include <vector>
#include "LFO Picker.h"

class VectorChords
{
public:
    
    //setting the sample rate
    void setSampleRate(float sampleRate_)
    {
        sampleRate = sampleRate_;
    }
    
    //setting the fundamental
    void setMidiFundamental(float midiFundamental_)
    {
        fundamentalMidi = midiFundamental_;
        
    }
    
    void setOscType(float oscType_)
    {
        oscType = oscType_;
        
    }
    
    void setChordType(int _chordType)
    {
        chordType = _chordType;
        
        //major Triad
        if(chordType == 1)
        {
            increment[0] = {4};
            increment[1] = {3};
            increment[2] = {0};
        }
        
        //major 7th
        if(chordType == 2)
        {
            increment[0] = {4};
            increment[1] = {3};
            increment[2] = {4};
        }
        
        
        //minor Triad
        if(chordType == 3)
        {
            increment[0] = {3};
            increment[1] = {4};
            increment[2] = {0};
        }
        
        //minor 7th
        if(chordType == 4)
        {
            increment[0] = {3};
            increment[1] = {4};
            increment[2] = {3};
        }
      
    }
    
    
    //clearing the vector
    void clear()
    {
        chordVector.clear();
    }
    
    //populating the vector
    void setIntervalOscillators()
    {
        // add oscillators to the vector and set samplerate and frequencies
        for (int i = 0; i < intervalCount; i++)
        {
            // add an oscillator to the vector (increasing the vector size by 1)
            chordVector.push_back(LFO_Picker());
            
            // set the sample rate of the newly added SinOsc
            chordVector[i].setSampleRate(sampleRate);
            
         int newMidi = fundamentalMidi + increment[i];
            
        // make midi into frequencies
        float intervals = juce::MidiMessage::getMidiNoteInHertz(newMidi);
            chordVector[i].setFrequency(intervals);
        }
        
    }
    
    float process()
    {
            // create variable to store the combined output of all oscillators
            float additiveOutput = 0;
            
            // loop through all the oscillators in the vector
            for (int j = 0; j < chordVector.size(); j++)
            {
                // sum up the oscillator outputs into one output mix
                additiveOutput += chordVector[j].process(oscType) / float(intervalCount);
            }
        
         // return the output mix
        return additiveOutput;
    }
   
    
private:
    
    std::vector<LFO_Picker> chordVector;
    float sampleRate;
    float fundamentalMidi;
    float intervalCount = 3;
    int chordType;
    int oscType;
    int increment[3] = {4, 3, 0};
    
};

#endif 

