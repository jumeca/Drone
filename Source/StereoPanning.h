#pragma once
#include "Oscillator.h"
#include <vector>

class StereoPanning {
public:

    void setThreshold(float _threshold) {
        threshold = _threshold;
    }

    /**
    * Set up panning values
    * @param sampleRate sample rate in Hz
    * @param panFreq frequency of the panning oscillator in Hz
    * @param modFreq frequency of the phase modulation in Hz
    * @param threshold limits the panning (0.0 - 0.3);
    */
    void setUp(float sampleRate, float panFreq, float modFreq, float threshold) {
        // Panning oscillator
        panning.setSampleRate(sampleRate);
        panning.setFrequency(panFreq);

        // Modulating oscillator
        modulator.setSampleRate(sampleRate);
        modulator.setFrequency(modFreq);
        modulator.setPhase(0.25f);

        // Phase modulation
        panning.setPhase(modulator.process());

        // Set threshold
        setThreshold(threshold);
        
        // Initialize stereo vector to 0
        for(int i=0; i<2;i++)
            stereo.push_back(0.0f);
    }

    /**
    * Panning process.
    * 
    * Update the loudness of the left and right in stereo.
    */
    void process() {
        
        // Update the phase
        panning.setPhase(modulator.process());

        // Make the ouput be a positive value
        float output = (panning.process() + 1.0f) * 0.5f;

        // Threshold limitation
        if (output <= threshold) 
            output = threshold;
        
        stereo[0] = output;                 // Right channel
        stereo[1] = 1.0f - output;          // Left channel
        
        if (stereo[1] <= threshold)
            stereo[1] = threshold;
    }
    
    /**
    * Right channel loudness / gain
    * @return right channel gain
    */
    float getRightPanning() {
        return stereo[0];
    }

    /**
    * Left channel loudness / gain
    * @return left channel gain
    */
    float getLeftPanning() {
        return stereo[1];
    }
     

private:
    std::vector<float> stereo;   // Stereo samples

    SineOsc panning;             // Panning oscillator
    SineOsc modulator;           // Modulating oscillator

    float threshold;             // Threshold must be between 0.0 and 1.0
};