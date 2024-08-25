//
//  Arpeggiator.h
//  JUCE SYNTH
//
//  Created by Alexandru Vasile on 20/12/2022.
//

#ifndef Arpeggiator_h
#define Arpeggiator_h

class Arpeggiator
{
public:
    
    //to be applied in the start Note
    void setStartArpeggiator(int _triggerArp)
    {
       if(_triggerArp == 1)
       {
           startArp = true;
       }
       else
       {
           startArp = false;
       }
    }
    
    void StopArpeggiator()
    {
        startArp = false;
    }
    
    //set Arp interval ins seconds and Num Samples
    void setNumSamples(float _numSamples)
    {
        numSamples = _numSamples;
    }
    
    //set Arp interval ins seconds and Num Samples
    void setArpInterval(float _ArpI)
    {
        N = _ArpI * numSamples;
    }
    
    //set from a selection of 4 chord types
    void setChordType(int _chordType)
    {
        chordType = _chordType;
        
        //major Triad
        if(_chordType == 1)
        {
            increment_1 = 4;
            increment_2 = 3;
            increment_3 = 4;
        }
        
        //major 7th
        if(_chordType == 2)
        {
            increment_1 = 4;
            increment_2 = 3;
            increment_3 = 4;
        }
        
        
        //minor Triad
        if(_chordType == 3)
        {
            increment_1 = 3;
            increment_2 = 4;
            increment_3 = 3;
        }
        
        //minor 7th
        if(_chordType == 4)
        {
            increment_1 = 3;
            increment_2 = 4;
            increment_3 = 3;
        }
        
    }
    
    //to be applied in the render next block
    int process(int _midiIn)
    {
        if(startArp == true)
        {
            T++;
            
            if(T < N)
            {
                midiReturn = _midiIn;
            }
            
            if(T >= N && T < (N * 2))
            {
                midiReturn = _midiIn + increment_1;
            }
                
            if(T >= (N * 2) && T < (N * 3))
            {
                midiReturn = _midiIn + increment_2;
            }
            
            //if chord lengh = 1 then do a triad instead of a 7th
            if(T == (N * 3) && chordType == 1)
            {
                T = 0;
            }
            
            //if chord lengh = 3 then do a triad instead of a 7th
            if(T == (N * 3) && chordType == 3)
            {
                T = 0;
            }
            
                
            if(T >= (N * 3) && T < (N * 4))
            {
                midiReturn = _midiIn + increment_3;
            }
            
            //reset after 7th chords
            if(T == (N * 4))
            {
                T = 0;
            }
        }
        else
        {
            T = 0;
            midiReturn = _midiIn;
        }
        
        return midiReturn;
    }
    
    
    
private:
    int T = 0;
    int chordType = 1;
    int increment_1;
    int increment_2;
    int increment_3;
    float N = 0;
    float numSamples;
    bool startArp = false;
    int midiReturn;
    
};





#endif /* Arpeggiator_h */
