/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "OSC_Generator.h"
#include "LFO_Generator.h"
#include "Probability_GATE.h"
#include "AHDSR_reloading.h"
#include "Delay_Class.h"
#include "Distortion_Class.h"
#include "Vector_Harmonics_Generator.h"

//==============================================================================
/**
*/
class Assignment_2AudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    Assignment_2AudioProcessor();
    ~Assignment_2AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    juce::AudioProcessorValueTreeState parameters;
    
    juce::Reverb reverb;
    juce::IIRFilter filterOne, filterTwo, filterThree;
    SinOsc sineOne, sineTwo;
    TriOsc triOne;
    SawOsc sawOne;
    SquareOsc squareOne, burst;
    
    SinLFO lfoOne, lfoThree, sinLFO, lfoFour;
    SquareLFO lfoTwo;
    SawLFO modLFO1, lfoFive;
    SquareLFO modLFO2;
    TriLFO modLFO3;
    
    ProbabilityGate gateOne;
    AHDSR envelopeOne, envelopeTwo, envelopeThree;
    DelayLine delayOne, delayTwo;
    VectorHarmonics vector;
    
    Distortion disto;
    
    int timeV;
    int T;
    float sr;
    
    std::atomic<float>* CutoffParam1;
    std::atomic<float>* CutoffParam2;
    std::atomic<float>* CutoffParam3;
    
    std::atomic<float>* TriggerParameter;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Assignment_2AudioProcessor)
};
