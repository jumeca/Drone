/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include "Oscillator.h"
#include "DelayLine.h"
#include "AdditiveSynth.h"
#include "StereoPanning.h"
#include "FilterSynth.h"

//==============================================================================
/**
*/
class DroneAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    DroneAudioProcessor();
    ~DroneAudioProcessor() override;

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
    float sampRate;                           // Sample rate in Hz
    float gain = 0.85f;                       // Gain
    
    SquareOsc triggerMidi;                    // Pulse to trigger midi change
    
    AdditiveSynth addSynth;                   // Set of oscillators for additive synthesis

    FilterSynth filter;                       // Filter and amplitude modulation
    DelayLine delay;                          // Delay

    StereoPanning panning;                    // Panning

    juce::Random random;                      // Variable to perform random operations
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DroneAudioProcessor)
};
