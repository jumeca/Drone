/*
  ==============================================================================

	Oscillator.cpp
	Created: 2 Mar 2021 6:54:49pm
	Author:  jumer

  ==============================================================================
*/

#include "Oscillator.h"

void Phasor::setSampleRate(float _sampleRate) {
	sampleRate = _sampleRate;
}

float Phasor::getSampleRate() {
	return sampleRate;
}


void Phasor::setFrequency(float _frequency) {
	frequency = _frequency;
	// Make sure that the sample rate is set
	if (!sampleRate)
		phaseDelta = frequency / sampleRate;
}

float Phasor::getFrequency() {
	return frequency;
}

void Phasor::setPhase(float _phase) {
	phase = _phase;
}

float Phasor::getPhase() {
	return phase;
}

float Phasor::process() {

}
/// this function is the one that we will replace in the classes that inherit from Phasor



float Phasor::output(float phase)
{
	return phase;
}

/// update the phase and output the next sample from the oscillator
float Phasor::process()
{
	phase += phaseDelta;

	if (phase > 1.0f)
		phase -= 1.0f;

	return output(phase);
}


float SineOsc::output(float phase) {
	return std::sin(2 * M_PI * phase);
}

float TriangleOsc::output(float phase) {
	return fabsf(phase - 0.5f) - 0.5f;
}

float SawOsc::output(float phase) {
	return phase - 0.5f;
}

float SquareOsc::output(float phase) {
	float output = 0.5f;
	if (phase > 0.5)
		output = -0.5f;
	return output;
}

void SquareOsc::setPulseWidth(float _pulseWidth) {
	pulseWidth = _pulseWidth;
}
