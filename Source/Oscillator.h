/*
  ==============================================================================

    Oscillator.h
    Created: 1 Mar 2021 8:35:31pm
    Author:  jumer

  ==============================================================================
*/

#pragma once

#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#ifndef M_PI
#define M_PI  3.1415926535897932384626433832795
#endif

/**
* Implementation of a phasor that represents a basic oscillator.
* 
* It is used as a parent class to set up the common properties and methods for most oscillators.
*/
class Phasor {

public:
    /** Getters and setters */

    void setSampleRate(float _sampleRate) {
        sampleRate = _sampleRate;
    }
    float getSampleRate() {
        return sampleRate;
    }

    void setFrequency(float _frequency) {
        frequency = _frequency;
        phaseDelta = frequency / sampleRate;
    }
    float getFrequency() {
        return frequency;
    }

    void setPhase(float _phase) {
        phase = _phase;
    }
    float getPhase() {
        return phase;
    }

    /**
    * Update the phase for each sample and process it to return the oscillator's output.
    * @return processed sample
    */
    float process() {
        // Increment the phase of the phasor by its corresponding phasedelta for each sample
        phase += phaseDelta;

        // Make the phase to be in the range 0.0 - 1.0
        if (phase > 1.0f)
            phase -= 1.0f;

        // Process the current phase to return the corresponding oscillator's sample
        return output(phase);
    }

    /**
    * Process the phase to return the corresponding oscillator's sample.  
    * This function will be overwritten / redefined in the classes that inherit from Phasor.
    * @param phase sample's phase
    * @return oscillator's sample
    */
    virtual float output(float phase) {
        return phase;
    }

private:
    float sampleRate;                   // Sample rate in Hz
    float frequency;                    // Frequency in Hz
    float phase = 0.0f;                 // Phase of the phasor (0.0 - 1.0)
    float phaseDelta;                   // How much increment the phase by for each sample

};


/**
* Implementation of a sine wave oscillator
*/
class SineOsc : public Phasor {
    float output(float phase) override {
        return sin(2 * M_PI * phase);
    }
};


/**
* Implementation of a triangular wave oscillator
*/
class TriangleOsc : public Phasor {
    float output(float phase) override {
        return fabsf(phase - 0.5f) - 0.5f;
    }
};


/**
* Implementation of a saw wave oscillator
*/
class SawOsc : public Phasor {
    float output(float phase) override {
        return phase - 0.5f;
    }
};


/**
* Implementation of a square wave oscillator
*/
class SquareOsc : public Phasor {

public:
    float output(float phase) override {
        float output = 0.5f;
        if (phase > 0.5)
            output = -0.5f;
        return output;
    }
    void setPulseWidth(float _pulseWidth) {
        pulseWidth = _pulseWidth;
    }

private:
    float pulseWidth;           // Positive part of a square wave
};
#endif