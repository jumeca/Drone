#pragma once
#include "Oscillator.h"

class FilterSynth {

public:

	/**
	* Set up the filter synth parameters
	* @param sampleRate sample rate in Hz
	* @param triFreq frequency of the triangular wave oscillator used for modulating the sample to filter
	* @param freqLfo1 LFO cutoff frequency in Hz
	* @param cutoff cutoff frequency of the low pass filter
	* @param freqLfo2 LFO resonance frequency in Hz
	* @param resonance resonance frequency of the low pass filter
	*/
	void setUp(float sampleRate, float triFreq, float freqLfo1, float cutoff, float freqLfo2, float resonance) {
		sampRate = sampleRate;

		// Triangular wave oscillator - filter
		triOsc.setSampleRate(sampleRate);
		triOsc.setFrequency(triFreq);

		// LFO1 to control cutoff
		lfoCutoff.setSampleRate(sampleRate);
		lfoCutoff.setFrequency(freqLfo1);
		lfoCutoff.setPhase(0.55f);

		// LFO2 to control resonance
		lfoResonance.setSampleRate(sampleRate);
		lfoResonance.setFrequency(freqLfo2);
		lfoResonance.setPhase(0.77f);

		// Low pass filter
		filter.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, cutoff, resonance));
		filter.reset();
	}

	/**
	* Perform the filtering
	* @param sampleToFilter input sample
	* @return filtered sample
	*/
	float process(float sampleToFilter) {
		// Cutoff frequecy arund 1100 Hz with a LFO variation of 500 Hz
		float cutoff = lfoCutoff.process() * 500.0f + 1100.0f;
		// Resonance fluctuating between 10 and 20
		float resonance = (3.0f + lfoResonance.process()) * 5.0f;
		// Update low pass filter parameters
		filter.setCoefficients(juce::IIRCoefficients::makeLowPass(sampRate, cutoff, resonance));
		// Modulating input sample
		float sampleOut = triOsc.process()*sampleToFilter;
		// Filter sample
		float filteredSample = filter.processSingleSampleRaw(sampleOut);

		return filteredSample;
	}

private:
	TriangleOsc triOsc;			// Modulating triangular oscillator (amplitude modulation of input sample)
	SineOsc lfoCutoff;			// Cutoff LFO
	SineOsc lfoResonance;		// Resonance LFO
	juce::IIRFilter filter;		// Filter
	
	float sampRate;				// Sample rate
};