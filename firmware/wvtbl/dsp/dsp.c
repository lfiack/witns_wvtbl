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
void dsp_process(h_dsp_t * h_dsp)
{
    // Compute pitch voltage
    float pitch = h_dsp->params[PITCH_PARAM];
    pitch += h_dsp->inputs[PITCH_INPUT];
    pitch = clamp(pitch, -4.f, 4.f);        // Not quite sure about those magic numbers

    // Compute X and Y voltages
    float param_x = h_dsp->params[X_PARAM];
    param_x += h_dsp->inputs[X_INPUT];
    param_x /= 10.f;
    param_x = clamp(param_x, 0.f, 1.f);

    float param_y = h_dsp->params[Y_PARAM];
    param_y += h_dsp->inputs[Y_INPUT];
    param_y /= 10.f;
    param_y = clamp(param_y, 0.f, 1.f);

    // Compute the frequency in Hz
    float freq = FREQ_C4 * pow(2.f, pitch); // Not sure about that either

    // Accumulate the phase
    h_dsp->phase += freq / h_dsp->sample_frequency;
    if (h_dsp->phase >= 1.f)
        h_dsp->phase -= 1.f;

    // Compute waveforms
    float sine = sin(2.f * M_PI * h_dsp->phase);
    float saw; // = (h_dsp->phase) * 2.f - 1.f;
    float tri;
    float square;
    if (h_dsp->phase < 0.25f)
    {
        saw = h_dsp->phase * 2.f;
        tri = h_dsp->phase * 4.f;
        square = +1.f;
    }
    else if (h_dsp->phase < 0.5f)
    {
        saw = h_dsp->phase * 2.f;
        tri = 1.f + (0.25f - h_dsp->phase) * 4.f;
        square = +1.f;
    }
    else if (h_dsp->phase < 0.75f)
    {
        saw = h_dsp->phase * 2.f - 2.f;
        tri = 1.f + (0.25f - h_dsp->phase) * 4.f;
        square = -1.f;
    }
    else
    {
        saw = h_dsp->phase * 2.f - 2.f;
        tri = (h_dsp->phase - 0.75f) * 4.f - 1.0f;
        square = -1.f;
    }

    // Generates partial output
#ifdef WEIRDO
    // Gives funny results that I want to ear
    float sine_out = ((1.f - param_x) * sine + (1.f - param_y) * sine) / 2.f;
    float tri_out = (param_x * tri + (1.f - param_y) * tri) / 2.f;
    float square_out = ((1.f - param_x) * square + param_y * square) / 2.f;
    float saw_out = (param_x * saw + param_y * saw) / 2.f;
#endif  // WEIRDO

    float sine_out = (1.f - param_x) * (1.f - param_y) * sine;
    float tri_out = param_x * (1.f - param_y) * tri;
    float square_out = (1.f - param_x) * param_y * square;
    float saw_out = param_x * param_y * saw;

    // Mixed output
    float out = sine_out + tri_out + square_out + saw_out;

    h_dsp->outputs[AUDIO_OUTPUT] = (5.f * out);
}