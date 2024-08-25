/*
  ==============================================================================

    MySynthesiser.h
    Created: 7 Mar 2020 4:27:57pm
    Author:  Tom Mudd

  ==============================================================================
*/

#pragma once

#include "OSC_Generator.h"
#include "Distortion_Class.h"
#include "Arpeggiator.h"
#include "LFO Picker.h"
//#include "Vector_Chord_Generator.h"

// ===========================
// ===========================

class MySynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote      (int) override      { return true; }
    //--------------------------------------------------------------------------
    bool appliesToChannel   (int) override      { return true; }
};




// =================================
// =================================
// Synthesiser Voice - your synth code goes in here

/*!
 @class MySynthVoice
 @abstract struct defining the DSP associated with a specific voice.
 @discussion multiple MySynthVoice objects will be created by the Synthesiser so that it can be played polyphicially
 
 @namespace none
 @updated 2019-06-18
 */
class MySynthVoice : public juce::SynthesiserVoice
{
public:
    
    MySynthVoice() {}
    
    
    void init(float sampleRate)
    {
        sr = sampleRate;
        
        //set sample rate for oscillator
        tri.setSampleRate(sampleRate);
        sin.setSampleRate(sampleRate);
        square.setSampleRate(sampleRate);
        saw.setSampleRate(sampleRate);
        
        detuneTri.setSampleRate(sampleRate);
        detuneSin.setSampleRate(sampleRate);
        detuneSquare.setSampleRate(sampleRate);
        detuneSaw.setSampleRate(sampleRate);
        
        lfo_1.setSampleRate(sampleRate);
        lfo_2.setSampleRate(sampleRate);
        lfo_3.setSampleRate(sampleRate);
        lfo_4.setSampleRate(sampleRate);
        
        
        modLFO_1.setSampleRate(sampleRate);
        modLFO_2.setSampleRate(sampleRate);
        modLFO_3.setSampleRate(sampleRate);
        modLFO_4.setSampleRate(sampleRate);
        
        
        env.setSampleRate(sampleRate);
        
        ///Filter initialisation
        filterOne.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, 660.0f, 5.0f));
        //filterOne.reset();
        
        filterTwo.setCoefficients(juce::IIRCoefficients::makeBandPass(sampleRate, 1000.0f, 1.0f));
        //filterTwo.reset();
        
        filterThree.setCoefficients(juce::IIRCoefficients::makeHighPass(sampleRate, 2000.0f, 2.0f));
        //filterThree.reset();
        
        
        //Arpeggiator
        arp.setStartArpeggiator(0);
        arp.setChordType(1);
        arp.setNumSamples(sampleRate);
        
        
        ///Vector harmonics generator initialisation
        //vectorChords.setSampleRate(sampleRate);
        
    }
    
    void setDetune(std::atomic<float>* _detuneTri, std::atomic<float>* _detuneSin, std::atomic<float>* _detuneSquare, std::atomic<float>* _detuneSaw, std::atomic<float>* _midiTransposeD, std::atomic<float>* _midiTransposeU)
    {
        detuneAmountOne = _detuneTri;
        detuneAmountTwo = _detuneSin;
        detuneAmountThree = _detuneSquare;
        detuneAmountFour = _detuneSaw;
        midiTransposeD = _midiTransposeD;
        midiTransposeU = _midiTransposeU;
        
    }
    
    void connectEnvelopeParams(std::atomic<float>* _attackParam, std::atomic<float>* _decayParam, std::atomic<float>* _sustainParam, std::atomic<float>* _releaseParam)
    {
        attack = _attackParam;
        decay = _decayParam;
        sustain = _sustainParam;
        release = _releaseParam;
    }
    
    void connectMixParams(std::atomic<float>* _triGain, std::atomic<float>* _sinGain, std::atomic<float>* _squareGain, std::atomic<float>* _sawGain, std::atomic<float>* _noiseGain)
    {
        triGain = _triGain;
        sinGain = _sinGain;
        squareGain = _squareGain;
        sawGain = _sawGain;
        noiseGain = _noiseGain;
    }
    
    void connectOscModParrams(std::atomic<float>* _triggerSinMod, std::atomic<float>* _triggerTriMod, std::atomic<float>* _triggerSquareMod, std::atomic<float>* _triggerSawMod)
    {
        triggerSinMod = _triggerSinMod;
        triggerTriMod = _triggerTriMod;
        triggerSquareMod = _triggerSquareMod;
        triggerSawMod = _triggerSawMod;
    }
    
    void connectFilterParams(std::atomic<float>* _filterParam1, std::atomic<float>* _filterParam2, std::atomic<float>* _filterParam3, std::atomic<float>* _osc_1_filter, std::atomic<float>* _osc_2_filter, std::atomic<float>* _osc_3_filter, std::atomic<float>* _osc_4_filter, std::atomic<float>* _Q_1, std::atomic<float>* _Q_2, std::atomic<float>* _Q_3, std::atomic<float>* _filterModTrigger, std::atomic<float>* _noise_filter)
    {
        cutoffParam1 = _filterParam1;
        cutoffParam2 = _filterParam2;
        cutoffParam3 = _filterParam3;
        osc_1_filter = _osc_1_filter;
        osc_2_filter = _osc_2_filter;
        osc_3_filter = _osc_3_filter;
        osc_4_filter = _osc_4_filter;
        noise_filter = _noise_filter;
        Q_1 = _Q_1;
        Q_2 = _Q_2;
        Q_3 = _Q_3;
        filterModTrigger = _filterModTrigger;
    }
    
    
    void connectDistortParams(std::atomic<float>* _triggerDisto, std::atomic<float>* _distoGain)
    {
        triggerDisto = _triggerDisto;
        distoGain = _distoGain;
    }
    
    
    void connectLFOParams(std::atomic<float>* _lfo_1_freq, std::atomic<float>* _lfo_1_depth, std::atomic<float>* _lfo_1_offset, std::atomic<float>* _lfo_1_choice, std::atomic<float>* _lfo_2_freq, std::atomic<float>* _lfo_2_depth, std::atomic<float>* _lfo_2_offset, std::atomic<float>* _lfo_2_choice, std::atomic<float>* _lfo_3_freq, std::atomic<float>* _lfo_3_depth, std::atomic<float>* _lfo_3_offset, std::atomic<float>* _lfo_3_choice, std::atomic<float>* _lfo_4_freq, std::atomic<float>* _lfo_4_depth, std::atomic<float>* _lfo_4_offset, std::atomic<float>* _lfo_4_choice)
    {
        lfo_1_freq = _lfo_1_freq;
        lfo_1_depth = _lfo_1_depth;
        lfo_1_offset = _lfo_1_offset;
        lfo_1_choice = _lfo_1_choice;
        lfo_2_freq = _lfo_2_freq;
        lfo_2_depth = _lfo_2_depth;
        lfo_2_offset = _lfo_2_offset;
        lfo_2_choice = _lfo_2_choice;
        lfo_3_freq = _lfo_3_freq;
        lfo_3_depth = _lfo_3_depth;
        lfo_3_offset = _lfo_3_offset;
        lfo_3_choice = _lfo_3_choice;
        lfo_4_freq = _lfo_4_freq;
        lfo_4_depth = _lfo_4_depth;
        lfo_4_offset = _lfo_4_offset;
        lfo_4_choice = _lfo_4_choice;
    }
    
    void connectModLfoParams(std::atomic<float>* _modLFO_1_freq, std::atomic<float>* _modLFO_2_freq, std::atomic<float>* _modLFO_3_freq, std::atomic<float>* _modLFO_4_freq, std::atomic<float>* _modLFO_1_On, std::atomic<float>* _modLFO_2_On, std::atomic<float>* _modLFO_3_On, std::atomic<float>* _modLFO_4_On, std::atomic<float>* _modLFO_1_Choice, std::atomic<float>* _modLFO_2_Choice, std::atomic<float>* _modLFO_3_Choice, std::atomic<float>* _modLFO_4_Choice)
    {
        //MOD LFO's
        modLFO_1_freq = _modLFO_1_freq;
        modLFO_2_freq = _modLFO_2_freq;
        modLFO_3_freq = _modLFO_3_freq;
        modLFO_4_freq = _modLFO_4_freq;
        
        
        modLFO_1_On = _modLFO_1_On;
        modLFO_2_On = _modLFO_2_On;
        modLFO_3_On = _modLFO_3_On;
        modLFO_4_On = _modLFO_4_On;
       
        
        modLFO_1_Choice = _modLFO_1_Choice;
        modLFO_2_Choice = _modLFO_2_Choice;
        modLFO_3_Choice = _modLFO_3_Choice;
        modLFO_4_Choice = _modLFO_4_Choice;
    }
   
    
    void connectArpeggiatorParams(std::atomic<float>* _arpTrigger, std::atomic<float>* _arpInterval, std::atomic<float>* _chordType)
    {
        arpTrigger = _arpTrigger;
        arpInterval = _arpInterval;
        chordType = _chordType;
    }
    
    //void connectVectorParams(std::atomic<float>* _vectChordGain, std::atomic<float>* _vectChordType, std::atomic<float>* _vectOscType)
    //{
       // vectChordGain = _vectChordGain;
       // vectChordType = _vectChordType;
       // vectOscType = _vectOscType;
    // }
    
};
    
    //--------------------------------------------------------------------------
    /**
     What should be done when a note starts

     @param midiNoteNumber
     @param velocity
     @param SynthesiserSound unused variable
     @param / unused variable
     */
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        
        playing = true;
        ending = false;
        
        midiNN = midiNoteNumber - *midiTransposeD + *midiTransposeU;
        
        vel = velocity;
        
        //Vector Chord Harmonizer
        //vectorChords.clear();
        //vectorChords.setMidiFundamental(midiNN);
        //vectorChords.setIntervalOscillators();
        
        
        
        //arp.StopArpeggiator();
        arp.setStartArpeggiator(*arpTrigger);
        
        
        tri.setFrequency(freq);
        sin.setFrequency(freq);
        square.setFrequency(freq);
        saw.setFrequency(freq);
        
        
        juce::ADSR::Parameters envParams;
        envParams.attack = *attack;
        envParams.decay = *decay;
        envParams.sustain = *sustain;
        envParams.release = *release;
        
        
        
        env.setParameters(envParams);
        
        //env.reset();
        env.noteOn();
        
    }
    //--------------------------------------------------------------------------
    /// Called when a MIDI noteOff message is received
    /**
     What should be done when a note stops

     @param / unused variable
     @param allowTailOff bool to decie if the should be any volume decay
     */
    void stopNote(float /*velocity*/, bool allowTailOff) override
    {
        env.noteOff();
        
    }
    
    //--------------------------------------------------------------------------
    /**
     The Main DSP Block: Put your DSP code in here
     
     If the sound that the voice is playing finishes during the course of this rendered block, it must call clearCurrentNote(), to tell the synthesiser that it has finished

     @param outputBuffer pointer to output
     @param startSample position of first sample in buffer
     @param numSamples number of smaples in output buffer
     */
    void renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        
        if (playing) // check to see if this voice should be playing
        {
            
            
            detuneTri.setFrequency(freq - *detuneAmountOne);
            detuneSin.setFrequency(freq - *detuneAmountTwo);
            detuneSquare.setFrequency(freq - *detuneAmountThree);
            detuneSaw.setFrequency(freq - *detuneAmountFour);
            
            // iterate through the necessary number of samples (from startSample up to startSample + numSamples)
            for (int sampleIndex = startSample;   sampleIndex < (startSample+numSamples);   sampleIndex++)
            {
                float envVal = env.getNextSample();
                
                //Arpeggiator
                
                arp.setChordType(*chordType);
                arp.setArpInterval(*arpInterval);
                arpReturn = juce::MidiMessage::getMidiNoteInHertz(arp.process(midiNN));
                freq = arpReturn;
                
                //Vector chord harmonizer
            
                //vectorChords.setChordType(*vectChordType);
                //vectorChords.setOscType(*vectOscType);
                //vectorChord = vectorChords.process() * *vectChordGain;
                
                    
                
                //MOD LFO's (need to set these params)
                modLFO_1.setFrequency(*modLFO_1_freq);
                modLFO_2.setFrequency(*modLFO_2_freq);
                modLFO_3.setFrequency(*modLFO_3_freq);
                modLFO_4.setFrequency(*modLFO_4_freq);
                
                
                modLfoFreq_1 = (modLFO_1.process(*modLFO_1_Choice) * 0.9f) + 0.1f;
                modLfoFreq_2 = (modLFO_2.process(*modLFO_2_Choice) * 0.9f) + 0.1f;
                modLfoFreq_3 = (modLFO_3.process(*modLFO_3_Choice) * 0.9f) + 0.1f;
                modLfoFreq_4 = (modLFO_4.process(*modLFO_4_Choice) * 0.9f) + 0.1f;
                
                
                //LFO's (need to set *modLFO_1_On params)
                
                if(*modLFO_1_On == 1)
                {
                    lfo_1.setFrequency(modLfoFreq_1 * *lfo_1_freq);
                }
                else
                {
                    lfo_1.setFrequency(*lfo_1_freq);
                }
                
                lfo_1.setDepthAndOffset(*lfo_1_depth * vel, *lfo_1_offset);
                
                if(*modLFO_2_On == 1)
                {
                    lfo_2.setFrequency(modLfoFreq_2 * *lfo_2_freq);
                }
                else
                {
                    lfo_2.setFrequency(*lfo_2_freq);
                }
                
                lfo_2.setDepthAndOffset(*lfo_2_depth * vel, *lfo_2_offset);
                
                if(*modLFO_3_On == 1)
                {
                    lfo_3.setFrequency(modLfoFreq_3 * *lfo_3_freq);
                }
                else
                {
                    lfo_3.setFrequency(*lfo_3_freq);
                }
                
                lfo_3.setDepthAndOffset(*lfo_3_depth * vel, *lfo_3_offset);
                
                if(*modLFO_4_On == 1)
                {
                    lfo_4.setFrequency(modLfoFreq_4 * *lfo_4_freq);
                }
                else
                {
                    lfo_4.setFrequency(*lfo_4_freq);
                }
                
                lfo_4.setDepthAndOffset(*lfo_4_depth * vel, *lfo_4_offset);
                
                
                float lfo_1_val = lfo_1.process(*lfo_1_choice);
                float lfo_2_val = lfo_2.process(*lfo_2_choice);
                float lfo_3_val = lfo_3.process(*lfo_3_choice);
                float lfo_4_val = lfo_4.process(*lfo_4_choice);
                
                
                //Oscillator modulation
                if(*triggerSinMod == 1.0f)
                {
                    sin.getPhaseModulator(lfo_1_val);
                }
                if(*triggerTriMod == 1.0f)
                {
                    tri.getAmpModulator(lfo_2_val);
                }
                if(*triggerSquareMod == 1.0f)
                {
                    square.getAmpModulator(lfo_3_val);
                }
                
                if(*triggerSawMod == 1.0f)
                {
                    saw.getAmpModulator(lfo_4_val);
                }
    
                
                //Filter bank
                if(*filterModTrigger == 1.0f)
                {
                     cutoffOne = ((lfo_1_val + 1.0f) / 2.0f) * *cutoffParam1 + 440.0f;
                     cutoffTwo = ((lfo_2_val + 1.0f) / 2.0f) * *cutoffParam2 + 250.0f;
                     cutoffThree = ((lfo_3_val + 1.0f) / 2.0f) * *cutoffParam3 + 500.0f;
                     QOne = ((lfo_4_val + 1.0f) / 2.0f) * *Q_1 + 1.0f;
                     QTwo = ((lfo_3_val + 1.0f) / 2.0f) * *Q_2 + 1.0f;
                     QThree = ((lfo_1_val + 1.0f) / 2.0f) * *Q_3 + 1.0f;
                }
                else
                {
                     cutoffOne = *cutoffParam1 + 120.0f;
                     cutoffTwo = *cutoffParam2 + 200.0f;
                     cutoffThree = *cutoffParam3 + 500.0f;
                     QOne =  *Q_1 + 1.0f;
                     QTwo =  *Q_2 + 1.0f;
                     QThree = *Q_3 + 1.0f;
                }
                
                filterOne.setCoefficients(juce::IIRCoefficients::makeLowPass(sr, cutoffOne, QOne));
                filterTwo.setCoefficients(juce::IIRCoefficients::makeBandPass(sr, cutoffTwo, QTwo));
                filterThree.setCoefficients(juce::IIRCoefficients::makeHighPass(sr,cutoffThree, QThree));
                
                
                //noise generator
                noise = (random.nextFloat() - 0.5) * *noiseGain;
                
                //multiple filter choice for oscillator 1
                
                if(*osc_1_filter == 0)
                {
                    //here add a mix of all oscillators AND their individual gain mix parameter, gain param between 0.0f and 1.0f
                    oscillator_1 = ((tri.process() + detuneTri.process()) * *triGain);
                }
                if(*osc_1_filter == 1)
                {
                    oscillator_1 = filterOne.processSingleSampleRaw(((tri.process() + detuneTri.process()) * *triGain));
                }
                if(*osc_1_filter == 2)
                {
                    oscillator_1 = filterTwo.processSingleSampleRaw(((tri.process() + detuneTri.process()) * *triGain));
                }
                if(*osc_1_filter == 3)
                {
                    oscillator_1 = filterThree.processSingleSampleRaw(((tri.process() + detuneTri.process()) * *triGain));
                }
                
                //multiple filter choice for oscillator 2
                
                if(*osc_2_filter == 0)
                {
                    oscillator_2 = ((sin.process() + detuneSin.process()) * *sinGain);
                }
                if(*osc_2_filter == 1)
                {
                    oscillator_2 = filterOne.processSingleSampleRaw(((sin.process() + detuneSin.process()) * *sinGain));
                }
                if(*osc_2_filter == 2)
                {
                    oscillator_2 = filterTwo.processSingleSampleRaw(((sin.process() + detuneSin.process()) * *sinGain));
                }
                if(*osc_2_filter == 3)
                {
                    oscillator_2 = filterThree.processSingleSampleRaw(((sin.process() + detuneSin.process()) * *sinGain));
                }
                
                //multiple filter choice for oscillator 3
                
                if(*osc_3_filter == 0)
                {
                    oscillator_3 = ((square.process() + detuneSquare.process()) * *squareGain);
                }
                if(*osc_3_filter == 1)
                {
                    oscillator_3 = filterOne.processSingleSampleRaw(((square.process() + detuneSquare.process()) * *squareGain));
                }
                if(*osc_3_filter == 2)
                {
                    oscillator_3 = filterTwo.processSingleSampleRaw(((square.process() + detuneSquare.process()) * *squareGain));
                }
                if(*osc_3_filter == 3)
                {
                    oscillator_3 = filterThree.processSingleSampleRaw(((square.process() + detuneSquare.process()) * *squareGain));
                }
                
                //multiple filter choice for oscillator 4
                
                if(*osc_4_filter == 0)
                {
                    oscillator_4 = ((saw.process() + detuneSaw.process()) * *sawGain);
                }
                if(*osc_4_filter == 1)
                {
                    oscillator_4 = filterOne.processSingleSampleRaw(((saw.process() + detuneSaw.process()) * *sawGain));
                }
                if(*osc_4_filter == 2)
                {
                    oscillator_4 = filterTwo.processSingleSampleRaw(((saw.process() + detuneSaw.process()) * *sawGain));
                }
                if(*osc_4_filter == 3)
                {
                    oscillator_4 = filterThree.processSingleSampleRaw(((saw.process() + detuneSaw.process()) * *sawGain));
                }
                
                //multiple filter choice for Noise
                
                if(*noise_filter == 0)
                {
                    noise_PF = (noise * *noiseGain);
                }
                if(*noise_filter == 1)
                {
                    noise_PF = filterOne.processSingleSampleRaw(noise * *noiseGain);
                }
                if(*noise_filter == 2)
                {
                    noise_PF = filterTwo.processSingleSampleRaw(noise * *noiseGain);
                }
                if(*noise_filter == 3)
                {
                    noise_PF = filterThree.processSingleSampleRaw(noise * *noiseGain);
                }
                

                //Mixing the oscillators and dropping the volume proportionally
                float oscillatorMix = (oscillator_1 + oscillator_2 + oscillator_3 + oscillator_4 + noise_PF) / (*triGain + *sinGain + *squareGain + *sawGain + *noiseGain) * vel;
                
                //*vectChordGain + vectorChord
                
                //Distortion Choice
                if(*triggerDisto == 1)
                {
                    distortionPost = disto.distortionClipper(oscillatorMix, *distoGain);
                }
                else
                {
                    distortionPost = oscillatorMix;
                }
                
                sample = distortionPost * 0.025 * envVal;
                
                
                // for each channel, write the currentSample float to the output
                for (int chan = 0; chan<outputBuffer.getNumChannels(); chan++)
                {
                // The output sample is scaled by 0.2 so that it is not too loud by default
                    outputBuffer.addSample (chan, sampleIndex, sample);
                }
                
                
                if(ending)
                {
                    if(envVal < 0.0001f)
                    {
                        clearCurrentNote();
                        playing = false;
                        env.reset();
                        arp.StopArpeggiator();
                    }
                }
            }
        }
    }
    
    
    
    //--------------------------------------------------------------------------
    void pitchWheelMoved(int) override {}
    //--------------------------------------------------------------------------
    void controllerMoved(int, int) override {}
    //--------------------------------------------------------------------------
    /**
     Can this voice play a sound. I wouldn't worry about this for the time being

     @param sound a juce::SynthesiserSound* base class pointer
     @return sound cast as a pointer to an instance of MySynthSound
     */
    bool canPlaySound (juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<MySynthSound*> (sound) != nullptr;
    }


    //--------------------------------------------------------------------------
private:
    //--------------------------------------------------------------------------
    // Set up any necessary variables here
    /// Should the voice be playing?
    bool playing = false;
    bool ending = false;
    
    //float synthOutput;
    
    float freq;
    float sr;
    
    float vel;
    
    float sample;
    
    float oscillator_1;
    float oscillator_2;
    float oscillator_3;
    float oscillator_4;
    float noise;
    float noise_PF;
    
    float cutoffOne;
    float cutoffTwo;
    float cutoffThree;
    float QOne;
    float QTwo;
    float QThree;
    
    
    //modd LFO's
    
    float modLfoFreq_1;
    float modLfoFreq_2;
    float modLfoFreq_3;
    float modLfoFreq_4;
    
    float distortionPost;

    float vectorChord;
    


    //set sample rate for oscillator



    Arpeggiator arp;
    
    
    LFO_Picker lfo_1, lfo_2, lfo_3, lfo_4, modLFO_1, modLFO_2, modLFO_3, modLFO_4;
    
    
    /// a random object for use in our test noise function
    juce::Random random;
    
    juce::IIRFilter filterOne, filterTwo, filterThree, filterFour;
    
    Distortion disto;
    
    TriOsc tri, detuneTri;
    SinOsc sin, detuneSin;
    SquareOsc square, detuneSquare;
    SawOsc saw, detuneSaw;
    
    juce::ADSR env;
    
    //VectorChords vectorChords;
   
    int midiNN;
    int arpReturn;
    
    //detune parameters
    
    std::atomic<float>* detuneAmountOne;
    std::atomic<float>* detuneAmountTwo;
    std::atomic<float>* detuneAmountThree;
    std::atomic<float>* detuneAmountFour;
    
    //Envelope parameters
    std::atomic<float>* attack;
    std::atomic<float>* decay;
    std::atomic<float>* sustain;
    std::atomic<float>* release;
    
    //mixer gain parameters
    std::atomic<float>* triGain;
    std::atomic<float>* sinGain;
    std::atomic<float>* squareGain;
    std::atomic<float>* sawGain;
    std::atomic<float>* noiseGain;
    
    //Oscillator modulation
    std::atomic<float>* triggerSinMod;
    std::atomic<float>* triggerTriMod;
    std::atomic<float>* triggerSquareMod;
    std::atomic<float>* triggerSawMod;
   
    //filter bank parameters
    std::atomic<float>* cutoffParam1;
    std::atomic<float>* cutoffParam2;
    std::atomic<float>* cutoffParam3;
    
    std::atomic<float>* osc_1_filter;
    std::atomic<float>* osc_2_filter;
    std::atomic<float>* osc_3_filter;
    std::atomic<float>* osc_4_filter;
    std::atomic<float>* noise_filter;
    
    std::atomic<float>* Q_1;
    std::atomic<float>* Q_2;
    std::atomic<float>* Q_3;
    
    std::atomic<float>* filterModTrigger;
    

    //distortion parameters
    std::atomic<float>* triggerDisto;
    std::atomic<float>* distoGain;
    
    //LFO parameters
    std::atomic<float>* lfo_1_freq;
    std::atomic<float>* lfo_1_depth;
    std::atomic<float>* lfo_1_offset;
    std::atomic<float>* lfo_1_choice;
    std::atomic<float>* lfo_2_freq;
    std::atomic<float>* lfo_2_depth;
    std::atomic<float>* lfo_2_offset;
    std::atomic<float>* lfo_2_choice;
    std::atomic<float>* lfo_3_freq;
    std::atomic<float>* lfo_3_depth;
    std::atomic<float>* lfo_3_offset;
    std::atomic<float>* lfo_3_choice;
    std::atomic<float>* lfo_4_freq;
    std::atomic<float>* lfo_4_depth;
    std::atomic<float>* lfo_4_offset;
    std::atomic<float>* lfo_4_choice;
    
   
    
    //MOD LFO's
    std::atomic<float>* modLFO_1_freq;
    std::atomic<float>* modLFO_2_freq;
    std::atomic<float>* modLFO_3_freq;
    std::atomic<float>* modLFO_4_freq;
    
    
    std::atomic<float>* modLFO_1_On;
    std::atomic<float>* modLFO_2_On;
    std::atomic<float>* modLFO_3_On;
    std::atomic<float>* modLFO_4_On;
    
    
    std::atomic<float>* modLFO_1_Choice;
    std::atomic<float>* modLFO_2_Choice;
    std::atomic<float>* modLFO_3_Choice;
    std::atomic<float>* modLFO_4_Choice;
    
    //Arp parameters
    std::atomic<float>* arpTrigger;
    std::atomic<float>* arpInterval;
    std::atomic<float>* chordType;
    
    //midi Transpose
    std::atomic<float>* midiTransposeD;
    std::atomic<float>* midiTransposeU;
    
    //Vector Chord
    //std::atomic<float>* vectChordGain;
    //std::atomic<float>* vectChordType;
    //std::atomic<float>* vectOscType;
};
