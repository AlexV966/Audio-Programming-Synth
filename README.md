# Audio-Programming-Synth
A synth plugin prototype coded in C++ in the JUCE framework 

The Fundamental components

As the fundamental components of the synth, four oscillators were created, a sine, a triangle, a saw and a square, alongside 4 adjacent and identical oscillators for detuning the original oscillators and a noise generator producing white noise for fullness and other sound design functions. Three filters, a low pass, band pass and high pass were produced. A general envelope is provided for the sum of all audio creating components. The idea of providing an envelope per oscillator was dropped fast due to a greater sound shaping reliance being put on the LFO section. A  bit-crushing component was provided as well for fullness.

The Extra Components

All extra components were created by using basic elements in a different arrangement of for loops, if statements, vectors, and arrays.

The simplest and most effective implementation was that of an if statement trigger system that can toggle on the distortion component, followed by the toggleOn/selectType arrangements which provided each Oscillator/detuneOscillator summed group with a selection of 3 different filters each and a bypass. This system was implemented similarly for the time based effects, panner and LFO’s. This step in itself sparked the creation of the LFO_Picker class which provides an LFO for each instance with the four selectable waveforms and depth/offset parameter capabilities, giving a far greater reach in terms of modulation variety. This LFO_Picker system could be toggled on separately for use in the cutoff of the filters or for individual use in phase modulation for the sine oscillator and amplitude modulation for the other oscillators.

On top of this, a similar LFO_Picker class instance was implemented as a second layer of modulation, to help in slowly modulating the frequency of the first layer and thus keeping it interesting for longer.

All oscillators were made controllable mainly by their own gain slider with a good real time gain compensation on their Sum.

The velocity was used to gently control both overall volume and to a lesser extent
Modulation. 
Two overall semitone based transpose sliders were added, one for any increase and one for any decrease.
A reverb with a wet and room size slider was implemented but more interestingly, a delay unit that functions both in mono and in stereo(more accurately dual mono) for the purpose of widening the stereo image has been added.

A panner system, powered by a double layer of LFOs adds an opportunity for some stereo effects.

The main midi based effects are the Arpeggiator and the Vector Chord generator.
The Arpeggiator is a prototype which interestingly is not powered by a phasor but rather by a layered timer system with a flexible time interval parameter and four chord types.

Whilst managing to change the notes accordingly, it takes more that one key press to have them change, which is an issue.

The last interesting newer element is the Vector chord generator which adds note intervals to the fundamental note to create a selection of chord types, all powered by a Vector of oscillators.

The Challenges

From the very beginning there were several issues and crashes, in particular regarding the implementation of the Value Tree state component and the way in which parameters were being passed onto the synth component. I tackled each and every error myself through the use of the debugger, analysis, trial and error and online research.
By far, the greatest challenge consisted of figuring out how to implement the time based effects into the processBlock rather than the renderNextBlock function, which singlehandedly required expanding my understanding of the juice::Buffer to achieve.


Desired Improvements

On top of the envelope follower mentioned in the beginning, there are certain improvements regarding the already implemented components that I deem necessary for the future of this plugin such as a sine function (power of) control, multiple choice menus instead of some sliders, wider use of velocity in delayTime/feedback and other Fx modulation, more widely implemented frequency modulation and vibrato (in the oscillator’s case), a harmonics generator option for the Vector Chord generator, vector based waveform selection instead of fixed oscillators and an Arpeggiator that changes the notes automatically based around the overall envelope.
