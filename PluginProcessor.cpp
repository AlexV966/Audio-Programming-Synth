/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Assignment_2AudioProcessor::Assignment_2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif

parameters(*this, nullptr, "ParamTreeIdentifier",
{
// ParameterLayout initialisation to go here.
    std::make_unique <juce::AudioParameterFloat > ("Cutoff_ParamOne", "Cutoff One", 440.0f, 770.0f, 770.0f),
    std::make_unique <juce::AudioParameterFloat > ("Cutoff_ParamTwo", "Cutoff Two", 440.0f, 770.0f, 770.0f),
    std::make_unique <juce::AudioParameterFloat > ("Cutoff_ParamThree", "Cutoff Three", 440.0f, 770.0f, 770.0f),
    std::make_unique <juce::AudioParameterChoice > ("choice", "Choice", juce::StringArray({"Choice 1", "Choice 2", "Choice 3"}), 0),
})
{
    //Constructor
    CutoffParam1 = parameters.getRawParameterValue("Cutoff_ParamOne");
    CutoffParam2 = parameters.getRawParameterValue("Cutoff_ParamTwo");
    CutoffParam3 = parameters.getRawParameterValue("Cutoff_ParamThree");
}

Assignment_2AudioProcessor::~Assignment_2AudioProcessor()
{
}

//==============================================================================
const juce::String Assignment_2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

void Assignment_2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    sr = sampleRate;
    
    ///Oscillator initialisation
    sineOne.setSampleRate(sampleRate);
    sineOne.setFrequency(440.0f);
    
    sineTwo.setSampleRate(sampleRate);
    sineTwo.setFrequency(440.0f);
    
    triOne.setSampleRate(sampleRate);
    triOne.setFrequency(660.0f);
    
    sawOne.setSampleRate(sampleRate);
    sawOne.setFrequency(225.0f);
    
    squareOne.setSampleRate(sampleRate);
    squareOne.setFrequency(330.0f);
    squareOne.setPulseWidth(0.5f);
    squareOne.getAmpModulator(1.0f);
    
    burst.setSampleRate(sampleRate);
    burst.setFrequency(1.0f);
    burst.setPulseWidth(0.1f);
   //========================================
    ///Mod LFO
    modLFO1.setSampleRate(sampleRate);
    modLFO1.setFrequency(0.8f);
    
    modLFO2.setSampleRate(sampleRate);
    modLFO2.setFrequency(0.5f);
    
    modLFO3.setSampleRate(sampleRate);
    modLFO3.setFrequency(1.0f);
    
    ///LFO initialisation
    lfoOne.setSampleRate(sampleRate);
    lfoOne.setFrequency(0.1f);
    lfoOne.setDepth(0.5f);
    lfoOne.setOffset(0.2f);
    
    lfoThree.setSampleRate(sampleRate);
    lfoThree.setFrequency(0.1f);
    
    lfoFour.setSampleRate(sampleRate);
    lfoFour.setFrequency(0.02f);
    
    lfoTwo.setSampleRate(sampleRate);
    lfoTwo.setFrequency(2.0f);
    lfoTwo.setPulseWidth(0.5);
    
    lfoFive.setSampleRate(sampleRate);
    lfoFive.setFrequency(0.2f);
    
    sinLFO.setSampleRate(sampleRate);
    sinLFO.setFrequency(0.05f);
    
    //========================================
    
    ///Probability Gate initialisation
    gateOne.setSampleRate(sampleRate);
    gateOne.setProbability(2.0);
    //========================================
    
    ///Envelope initialisation
    envelopeOne.setSampleRate(sampleRate);
    envelopeOne.setSustainLevel(0.3f);
    envelopeOne.setDurationAHDSR(10.0f, 5.0f, 5.0f, 10.0f, 10.0f, 10.0f);
    envelopeOne.setEnvelopeStart();
    
    envelopeTwo.setSampleRate(sampleRate);
    envelopeTwo.setSustainLevel(0.5f);
    envelopeTwo.setDurationAHDSR(5.0f, 5.0f, 10.0f, 5.0f, 7.0f, 20.0f);
    envelopeTwo.setEnvelopeStart();
    
    envelopeThree.setSampleRate(sampleRate);
    envelopeThree.setSustainLevel(0.5f);
    envelopeThree.setDurationAHDSR(2.0f, 0.0f, 4.0f, 1.0f, 4.0f, 20.0f);
    envelopeThree.setEnvelopeStart();
    //========================================
    
    ///Delay initialisation
    delayOne.setSizeInSamples(sampleRate);
    delayOne.setDelayTime(sampleRate * 0.25);
    
    delayTwo.setSizeInSamples(sampleRate);
    delayTwo.setDelayTime(sampleRate * 0.5);
    //========================================
    
    ///Vector harmonics generator initialisation
    vector.setSampleRate(sampleRate);
    vector.setOscCount(3);
    vector.setFundamental(220.0f);
    vector.setHarmonicsCount();
    timeV = 15;
    T = 1;
    //========================================
    
    ///Filter initialisation
    filterOne.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, 660.0, 5.0f));
    //filterOne.reset();
    
    filterTwo.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, 660.0, 5.0f));
    //filterTwo.reset();
    
    filterThree.setCoefficients(juce::IIRCoefficients::makeHighPass(sampleRate, 660.0, 2.0f));
    //filterThree.reset();
    //========================================
    
    ///Reverb initialisation
    juce::Reverb::Parameters reverbParams;
    reverbParams.dryLevel = 0.5f;
    reverbParams.wetLevel = 0.5f;
    reverbParams.roomSize = 0.99f;
    
    reverb.setParameters(reverbParams);
    
    reverb.reset();
    //========================================
}

void Assignment_2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
  
    // find out how many samples are in the block
    // e.g. 512
    int numberOfSamples = buffer.getNumSamples();
    
    // get pointers to both the left and the right channels in the buffer
    float* leftSamples = buffer.getWritePointer(0);
    float* rightSamples = buffer.getWritePointer(1);
    
    // DSP LOOP
    for (int i = 0; i < numberOfSamples; i++)
    {
        //A timer meant to refresh the oscillator vector
        T++;
        if(T >= timeV * sr)
        {
            T = 0;
            
            //clear the vector
            vector.clear();
            //add a randomized fundamental frequency
            vector.setFundamental(rand() % 400 + 101);
            //add a random OscCount
            vector.setOscCount(rand() % 4 + 1);
            
            //randomized harmonics count
            vector.setHarmonicsCount();
            
            //a randomized time for the timer
            timeV = rand() % 19 + 1;
        }
        
   //===========================================
        //A pulsed noise generator
        float noise = rand() % 1000 / 1000.0;
        noise -= 0.5;
        float pulsedNoise = noise * (burst.process() + 0.5);
    //===========================================
        
        //a straight sine with no depth or offset modulates the odds of a probability generator
        gateOne.setProbability(lfoThree.process());
        //a random nr. is generated, to be tested by the probability generator
        gateOne.setRandomNR();
        
    //===========================================
        //modulating the delay time of the delay class
        float newDelayTimeOne = (lfoOne.process() * 4000) + 5000;
        delayOne.setDelayTime(newDelayTimeOne);
       
        
        float newDelayTimeTwo = (lfoThree.process() * 4000) + 5000;
        delayTwo.setDelayTime(newDelayTimeTwo);
        delayTwo.modFeedback(lfoFour.process());
        
    //===========================================
        //modulating the frequency of a Triangle oscillator
        float triFrequency = (lfoTwo.process() * 660.0f) + 110.0f;
        triOne.setFrequency(triFrequency);
    //===========================================
        
        //modulating modulator frequencies
        float LFOFreqOne = (modLFO1.process() * 0.9f) + 0.1f;
        float LFOFreqTwo = (modLFO2.process() * 1.4f) + 0.1f;
        float LFOFreqThree = (modLFO3.process() * 0.9f) + 0.1f;
        
        lfoOne.setFrequency(LFOFreqOne);
        lfoTwo.setFrequency(LFOFreqTwo);
        lfoFive.setFrequency(LFOFreqThree);
        
    //===========================================
        //modulating the amplitude and phase of a sine oscillator
        sineOne.getAmpModulator(lfoOne.process());
        sineOne.getPhaseModulator(lfoTwo.process());
        
    //===========================================
        sineTwo.getAmpModulator(lfoTwo.process());
        sineTwo.getPhaseModulator(lfoOne.process());
        sineTwo.setPowerOf();
    //===========================================
        squareOne.getAmpModulator(lfoFive.process());
        sawOne.getAmpModulator(lfoTwo.process());
        
    //===========================================
        //define cutoff frequencies from lfo's
        float cutoffOne = ((sinLFO.process() + 1.0f) / 2.0f) * 330.0f + 440.0f;
        float cutoffTwo = ((lfoFour.process() + 1.0f) / 2.0f) * 330.0f + 250.0f;
        float cutoffThree = ((lfoFour.process() + 1.0f) / 2.0f) * 3000.0f + 500.0f;
        float QThree = ((sinLFO.process() + 1.0f) / 2.0f) * 4.0f + 1.0f;
        
        //float cutoffParam1;
        //float cutoffParam2;
        //float cutoffParam3;
        
        // apply the filter parameters
        filterOne.setCoefficients(juce::IIRCoefficients::makeLowPass(sr, *CutoffParam1, 1.0f));
        filterTwo.setCoefficients(juce::IIRCoefficients::makeLowPass(sr, *CutoffParam2, 2.0f));
        filterThree.setCoefficients(juce::IIRCoefficients::makeHighPass(sr, *CutoffParam3, QThree));
        //===========================================
        
        //mix components
        float componentOne = filterOne.processSingleSampleRaw(squareOne.process()) * 0.2;
        float componentTwo = filterThree.processSingleSampleRaw(gateOne.probabilityGate(pulsedNoise + delayOne.process(pulsedNoise))) * 0.5;
        float componentThree = envelopeOne.process(sawOne.process());
        float componentFour = filterTwo.processSingleSampleRaw(disto.distortionClipper(triOne.process(), lfoFour.process())) * 0.1;
        float componentFive = vector.process();
        float componentSix = envelopeTwo.process(sineOne.process());
        float componentSeven = envelopeThree.process(delayTwo.process(sineOne.process())) * 1.5;
        float componentEight = sineTwo.process();
        
        
        //creating the output mix
        float mix = componentOne + componentTwo + componentThree + componentFour + componentFive + componentSix + componentSeven + componentEight;
        float sample = mix * 0.1;
        
        
        
    
        leftSamples[i] = sample * 0.2;
        rightSamples[i] = sample * 0.2;
    }
    reverb.processStereo(leftSamples, rightSamples, numberOfSamples);
}

bool Assignment_2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Assignment_2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Assignment_2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Assignment_2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Assignment_2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Assignment_2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Assignment_2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Assignment_2AudioProcessor::getProgramName (int index)
{
    return {};
}

void Assignment_2AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================

void Assignment_2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Assignment_2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//==============================================================================
bool Assignment_2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Assignment_2AudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void Assignment_2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState(); std::unique_ptr<juce::XmlElement> xml (state.createXml()); copyXmlToBinary (*xml, destData);
}

void Assignment_2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
    if (xmlState->hasTagName (parameters.state.getType()))
    parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Assignment_2AudioProcessor();
}
