#pragma once
#include <vector>
#include "Oscillator.h"


class AdditiveSynth {
public:


	void setOscillatorCount(int _oscillatorCount) {
		oscillatorCount = _oscillatorCount;
	}

	int getOscillatorCount() {
		return oscillatorCount;
	}
	
	void setSampleRate(float _sampleRate) {
		sampRate = _sampleRate;
	}

	/**
	* Prepare the set of oscillators
	*
	* @param sampleRate in Hz
	* @param midiNote note
	*/
	void setUp(float sampleRate, int oscCount, float midiNote) {
		setSampleRate(sampRate);
		setOscillatorCount(oscCount);
		float frequency = midiToFrequency(midiNote);
		for (int i = 0; i < oscillatorCount; i++)
		{
			oscs.push_back(SineOsc());
			oscs[i].setSampleRate(sampleRate);
			float freq = (i + 1) * frequency;
			oscs[i].setFrequency(freq);
		}

	}

	/**
	* Process tha addition of oscillators applying overtone gain
	*/
	float process() {

		float output = 0.0f;
		for (int i = 0; i < oscillatorCount; i++)
		{
			float toneGain = 1.0f / (float) ((i+1)*2);

			output += toneGain * oscs[i].process();
		}
		return output / (float) oscillatorCount;

	}

	/**
	* Set frequencies (fundamental and overtones) by a given midi note
	* @param midi note midi
	*/
	void setFrequencies(float midiNote) {
		float frequency = midiToFrequency(midiNote);
		for (int i = 0; i < oscillatorCount; i++)
		{
			float freq = (i + 1) * frequency;
			oscs[i].setFrequency(freq);
		}
	}


private:
	/**
	* Convert MIDI pitches to frequencies in Hz
	*
	* @param note midi note (60 = middle C, 69 = 440 Hz)
	* @return frequency in Hz
	*/
	float midiToFrequency(float midiNote) {
		return pow(2, ((midiNote - 69) / 12)) * 440;
	}

	float sampRate;					  // Sample rate

	int oscillatorCount;			  // Number of oscillators
	std::vector<SineOsc> oscs;        // Oscillators vector
};