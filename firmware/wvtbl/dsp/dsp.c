#include "dsp.h"

#include "math.h"

float clamp (float voltage, float min, float max)
{
    float ret = voltage;

    if (ret < min) 
    {
        ret = min;
    }
    if (ret > max)
    {
        ret = max;
    }

    return ret;
}

void dsp_init(h_dsp_t * h_dsp)
{
    h_dsp->phase = 0.0f;
}

// Process one sample
// This function is called by the main loop at the sampling frequency
void dsp_process(h_dsp_t * h_dsp, uint32_t * dac_buffer, int32_t dac_buffer_size)
{
    // Compute pitch voltage
    float pitch = (float)(h_dsp->params[PITCH_PARAM])/12.f;
    // TODO add h_dsp->inputs[PITCH_INPUT]
    // TODO clamp

    // Compute oscillator frequency
    h_dsp->frequency = FREQ_C4 * pow(2.f, pitch); // Not sure about that either

    // Compute X and Y voltages
    // TODO

    // Wavetable interpolation
    // TODO
    
    // Transfered from 0 to DAC_BUFFER_SIZE/2 - 1
    for (int it = 0 ; it < dac_buffer_size ; it++)
    {
        // Accumulate the phase
        h_dsp->phase += h_dsp->frequency / h_dsp->sample_frequency;
        if (h_dsp->phase >= 1.f) h_dsp->phase -= 1.f;

        uint16_t index = (uint16_t)(h_dsp->phase * 4096);

        // TODO read a wavetable instead
        dac_buffer[it] = index;
    }
}