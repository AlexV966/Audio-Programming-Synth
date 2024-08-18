//
//  Delay_Class.h
//
//
//
//

#ifndef Header_h
#define Header_h

class DelayLine
{
public:
    
    ///set maximum size of delay line
    void setSizeInSamples(int newSize)
    {
        size = newSize; // store the new size
        
        if(data!= nullptr)
        {
            delete[] data;
        }
        data = new float[size]; //initialise our array
        
        for(int i=0; i < size; i++)
        {
            //set all values to 0
            data[i] = 0.0;
        }
    }
    
    ///set delay time in samples
    void setDelayTime(float newDelayTime)
    {
        delayTime = newDelayTime;
        //set the read index to be behind the write index
        readIndex = writeIndex - delayTime;
        
        if(readIndex < 0)
        {
            readIndex = readIndex + size;
        }
    }
    
    ///store a sample
    void writeDataToDelayLine(float inputSample)
    {
        
    }
    
    /// advance play and record head, wrapping as necessary
    void updateReadAndWriteHead()
    {
        
    }
    
    /// call every sample to:
    /// -- store the new sample
    /// --advance playheads
    /// --return the value at the read index
    float process(float inputSample)
    {
        //get value at green read index
        float outputSample = linearIntrpolation();
        
        //get value at red read index
        data[writeIndex] = inputSample + outputSample * feedback;
        
        //advance read and write indexes and wrap them back into the correct range
        readIndex += 1;
        
        if(readIndex >= size)
        {
            readIndex -= size;
        }
        
        writeIndex += 1;
        
        if(writeIndex >= size)
        {
            writeIndex -= size;
        }
        
        return outputSample;
    }
    
    float linearIntrpolation()
    {
        
        int indexA = int(readIndex);
        int indexB = indexA + 1;
        
        if(indexB > size)
            indexB -= size;
        
        float valA = data[indexA];
        float valB = data[indexB];
        
        float remainder = readIndex - indexA;
        float interpolatedSample = (1 - remainder) * valA + remainder * valB;
        return interpolatedSample;
    }
        
    //void setFeedback(float _feedback)
    //{
       // feedback = _feedback;
        //if (feedback < 0)
        //    feedback = 0;
        
        //if (feedback > 0.8)
        //    feedback = 0.8;
   // }
    
    void modFeedback(float LFOin_)
    {
        //a straight sine LFO converted to a range of 0 to 0.8
        dynamicFeedback = (LFOin_ + 1.0) * 0.4;
        
        feedback = dynamicFeedback;
        
        if (feedback < 0)
            feedback = 0;
        
        if (feedback > 0.8)
            feedback = 0.8;
    }
    
private:
    float readIndex = 0;   //read position as an index
    
    int writeIndex = 0; //write position as an index
    
    int size;      //maximum possible delay time
    
    float* data = nullptr;   //store input audio data
    
    int delayTime; //delay time in samples
    
    float feedback = 0.0f;
    
    float dynamicFeedback;
    
};

#endif 
