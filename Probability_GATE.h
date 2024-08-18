//
//  Probability_GATE.h
//
//
//
//

#ifndef Random_Envelope_Trigger_h
#define Random_Envelope_Trigger_h
#include <ctime>

class ProbabilityGate
{
public:
    
    //sets the sample rate
    void setSampleRate(float sampleRate_)
    {
        sampleRate = sampleRate_;
    }
    
    //sets the sine modulator and converts its range
    void setProbability(float sineModulatorIn)
    {
        //brings the sine modulator into the range 0. - 1.
        probability = (sineModulatorIn + 1.0f) / 2.0f;
    }
    
    //a self propagating timer
    void setRandomNR()
    {
        TI++;
        
        if(TI >= timeT * sampleRate)
        {
            TI = 0;
            
            //a random nr. is generated, to be tested in the probability generator
            randomNR = (rand() % 99 + 1) * 1.0f;
            
            timeT = rand() % 4 + 1;
        }
        
    }
    
    //a probability testing if statement that can either return the signal or nothing at all
    float probabilityGate(float signalIn)
    {
        if(randomNR <= (100.0 * probability))
        {
            return signalIn;
        }
        else
        {
            return 0.0f;
        }
    }
    
    
private:
    float sampleRate;
    float randomNR;
    float probability = 2;
    int timeT = 5;
    int TI = 1;
};




#endif 
