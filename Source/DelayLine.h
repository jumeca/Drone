#pragma once
class DelayLine {

public:

    ~DelayLine() {
        //clean up when the object is destroyed to avoid memory leaks
        delete buffer;      
    }

    /* Getters and setters */
    void setSize(int _size) {
        size = _size;
        buffer = new float[size];
        // Allocate 0 in buffer
        for (int i = 0; i < size; i++)
        {
            buffer[i] = 0.0f;
        }
    }

    int getSize() {
        return size;
    }


    void setDelayTimeInSamples(int _delayTimeInSamples) {
        delayTimeInSamples = _delayTimeInSamples;
        readPos = writePos - delayTimeInSamples;
        while (readPos < 0) {
            readPos += size;
        } 

    }

    int getDelayTimeInSamples() {
        return delayTimeInSamples;
    }

    void setFeedback(float _feedback) {
        if(feedback<1.0f)
            feedback = _feedback;
    }

    float getFeedback() {
        return feedback;
    }

    /**
    * Perform delay of input sample
    * @param inputSample sample to be delayed
    * @return delayed sample
    */
    float process(float inputSample) {

        // recall value from buffer
        float output = buffer[readPos];
        output = inputSample * (1.0f - feedback) + feedback * output;

        // store value
        buffer[writePos] = output;

        // increment write position
        writePos++;
        writePos = writePos % size;


        // increment read position
        readPos++;
        readPos = readPos % size;
        
        return output;
    }



private:
    float* buffer;              // Array of floats that defines the buffer where the delay samples are stored
    int size;                   // Size of the buffer

    int delayTimeInSamples;     // Delay time in samples
    int writePos = 0;           // Writing position within the buffer
    int readPos = 0;            // Reading position within the buffer

    float feedback = 0.5f;      // Amount of feedback in the delay
};