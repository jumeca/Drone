/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DroneAudioProcessor::DroneAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

DroneAudioProcessor::~DroneAudioProcessor()
{
}

//==============================================================================
const juce::String DroneAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DroneAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DroneAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DroneAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DroneAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DroneAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DroneAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DroneAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DroneAudioProcessor::getProgramName (int index)
{
    return {};
}

void DroneAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DroneAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Set up set of oscillators for additive synthesis
    addSynth.setUp(sampleRate, 10, 60);

    // Set up filter synth
    filter.setUp(sampleRate, 0.5f, 0.04f, 220.0f, 0.0025f, 5.0f);

    // Set up triggering of midi with a pulse made with a square oscillator
    triggerMidi.setSampleRate(sampleRate);
    triggerMidi.setFrequency(0.04f);
    triggerMidi.setPulseWidth(1 / (float) sampleRate);      //Trigger

    // Prepare delay line
    delay.setSize(sampleRate*10);
    delay.setDelayTimeInSamples(sampleRate*0.6f);
    delay.setFeedback(0.4f);

    // Set up stereo panning
    panning.setUp(sampleRate, 0.25f, 0.00357f,0.122f);

}

void DroneAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DroneAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DroneAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    float* leftChannel = buffer.getWritePointer(0);             // Array to write left channel samples inside the buffer
    float* rightChannel = buffer.getWritePointer(1);            // Array to write right channel samples inside the buffer
    int numSamples = buffer.getNumSamples();                    // Number of samples in the buffer

    /** DSP LOOP */
    for (int i = 0; i < numSamples; i++)
    {
        // Trigger a random midi note per triggerMidi pulse
        if (triggerMidi.process() > 0.0){
            float midi = random.nextFloat() * 4 + 60;
            addSynth.setFrequencies(midi);
        }
            
        // Additive synth 
        float sample = gain * addSynth.process();
        // Filter the sample
        sample =  filter.process(sample);
        // Apply delay to the sample
        sample = delay.process(sample);
        // Process the current stereo panning values
        panning.process();
        // Mixing the delayed sample with the panning
        leftChannel[i] = panning.getLeftPanning()*sample;
        rightChannel[i] = panning.getRightPanning() * sample;
    }
}

//==============================================================================
bool DroneAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DroneAudioProcessor::createEditor()
{
    return new DroneAudioProcessorEditor (*this);
}

//==============================================================================
void DroneAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DroneAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DroneAudioProcessor();
}
