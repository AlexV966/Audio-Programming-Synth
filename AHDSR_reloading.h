//
//  AHDSR_reloading.h
//
//
//
//

#ifndef ADSR_h
#define ADSR_h

class AHDSR
{
public:
    
    ///set duration in samples
    void setSampleRate(float newSampleRate)
    {
        sampleRate = newSampleRate;
    }
    
    ///set attack, hold, decay sustain and release times
    void setDurationAHDSR(float newA, float newH, float newD, float newS, float newR, float newT)
    {
        //attack time
        durationA = newA;
        
        //hold time
        durationH = newH;
        
        //decay time
        durationD = newD;
        
        //sustain time
        durationS = newS;
        
        //release time
        durationR = newR;
        
        //timer time
        durationT = newT;
        
        //creating an increment for phase changes
        incrementDeltaA = 1 / (durationA * sampleRate);
        incrementDeltaD = 1 / (durationD * sampleRate);
        incrementDeltaR = 1 / (durationR * sampleRate);
        
        //creating a custom duration in samples for each timer
        numberOfSamplesH = durationH * sampleRate;
        numberOfSamplesS = durationS * sampleRate;
        numberOfSamplesT = durationT * sampleRate;
    }
    
    ///set sustain level
    void setSustainLevel(float newSustain)
    {
        sustainLevel = newSustain;
        
        //limiting the range of the sustain level
        if(sustainLevel < 0.0f)
        {
            sustainLevel = 0.0f;
        }
        
        if(sustainLevel > 0.8f)
        {
            sustainLevel = 0.8f;
        }
    }
    
    ///starting an envelope cycle
    void setEnvelopeStart()
    {
        envelopeStart = true;
        envelopeEnd = false;
    }
    
    float process(float inSignal)
    {
        //attack ramp
        if(increment < peakValue && envelopeStart == true)
        {
            increment += incrementDeltaA;
        }
        
        //peak reached, switch to hold
        if(increment >= peakValue)
        {
            envelopeStart = false;
            holdStart = true;
        }
        
        //hold timer
        if(holdStart == true)
        {
            H += 1;
            
            if(H >= numberOfSamplesH)
            {
                holdStart = false;
                decayStart = true;
                H -= numberOfSamplesH;
            }
            
        }
        
        //decay ramp
        if(holdStart == false && decayStart == true)
        {
            increment -= incrementDeltaD;
        }
        
        //stopping the increment from falling further than the sustain level and switching to the sustain timer
        if(decayStart == true && increment <= sustainLevel)
        {
            decayStart = false;
            sustainStart = true;
            increment = sustainLevel;
        }
        
        //running the sustain timer
        if(sustainStart == true)
        {
            S += 1;
            
            if(S >= numberOfSamplesS)
            {
                sustainStart = false;
                releaseStart = true;
                S -= numberOfSamplesS;
            }
        }
        
        //once the sustain timer is done, switch to the release phase
        if(sustainStart == false && releaseStart == true)
        {
            increment -= incrementDeltaR;
        }
        
        //Ensuring the increment doesn't go into minus ranges before main duration is over and stopping the release
        if(releaseStart == true && increment <= 0.0f)
        {
            increment = 0.0f;
            releaseStart = false;
        }
        
        //declare envelope cycle to be over and send a message to the cycle timer
        if(envelopeStart == false && holdStart == false && decayStart == false && sustainStart == false && releaseStart == false)
        {
            envelopeEnd = true;
        }
        
        //start the cycle timer, generating a random time to elapse before refreshing all parameters and restarting the envelope cycle
        if(envelopeEnd == true)
        {
            T++;
                
            if(T >= numberOfSamplesT)
            {
                envelopeEnd = false;
                T -= numberOfSamplesT;
                
                ///AHDSR PARAMETERS REFRESH
                //Attack time in seconds
                float rA = rand() % 10 + 1;
                //Hold time in seconds(can be 0)
                float rH = rand() % 11;
                //Decay time in seconds
                float rD = rand() % 10 + 1;
                //Sustain time in seconds(can be 0)
                float rS = rand() % 11;
                //Release time in seconds
                float rR = rand() % 10 + 1;
                //a random nr between 1.0 - 20.0 for the pause timer
                float rT = (rand() % 10000) * 0.0019 + 1.0;
                
                //a sustain amp randomiser between 0.1 and 0.8
                float rSAMP = (rand() % 10000) * 0.00007f + 0.1f;
                
                setDurationAHDSR(rA, rH, rD, rS, rR, rT);
                setSustainLevel(rSAMP);
                setEnvelopeStart();
                envelopeStart = true;
                
            }
        }
        
        //returning the modulated result of the increment and input signal
        result = increment * inSignal;
        
        return result;
        
    }
    
private:
    float durationA;
    float durationD;
    float durationR;
    float durationH;
    float durationS;
    float durationT;
    float sampleRate;
    float increment = 0.0f;
    float peakValue = 1.0f;
    float sustainLevel = 0.0f;
    float incrementDeltaA;
    float incrementDeltaD;
    float incrementDeltaR;
    float numberOfSamplesH;
    float numberOfSamplesS;
    float numberOfSamplesT;
    float result;
    int H = 0;
    int S = 0;
    int T = 0;
    bool envelopeStart = false;
    bool holdStart = false;
    bool decayStart = false;
    bool sustainStart = false;
    bool releaseStart = false;
    bool envelopeEnd = false;
        
};

#endif 
