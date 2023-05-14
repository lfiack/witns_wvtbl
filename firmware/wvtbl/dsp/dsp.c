#include "dsp.h"

#include "math.h"

#include "table.h"

float fclamp (float voltage, float min, float max)
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

int32_t clamp (int32_t voltage, int32_t min, int32_t max)
{
    int32_t ret = voltage;

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

uint16_t interpolated_table[TABLE_SAMPLE];

// Process one sample
// This function is called by the main loop at the sampling frequency
void dsp_process(h_dsp_t * h_dsp, uint32_t * dac_buffer, int32_t dac_buffer_size)
{
    // Compute pitch voltage
    float pitch = (float)(h_dsp->params[PITCH_PARAM])/12.f;
    // C0 = 2V = 2380LSB ; C2 = 4V = 2818SB
    float voct_sub = 2380.f;                    // Compensate the offset
    float voct_div = (2818.f - 2380.f) / 2.f;   // Compensate the gain
    float voct = ((float)(h_dsp->inputs[PITCH_INPUT]) - voct_sub) / voct_div;
    pitch += voct;
    // TODO add h_dsp->inputs[PITCH_INPUT]
    // TODO clamp

    // Compute oscillator frequency
    h_dsp->frequency = FREQ_C4 * pow(2.f, pitch); // Not sure about that either

    // Compute X and Y voltages
    int32_t x_param = clamp((h_dsp->inputs[X_INPUT] + h_dsp->params[X_PARAM]), PARAM_MIN, PARAM_MAX);
    int32_t y_param = clamp((h_dsp->inputs[Y_INPUT] + h_dsp->params[Y_PARAM]), PARAM_MIN, PARAM_MAX);

    // Wavetable interpolation
    // TODO
    uint32_t x_threshold = 4096/(TABLE_X_LEN-1);
    uint32_t y_threshold = 4096/(TABLE_Y_LEN-1);
    uint32_t x_index = x_param/x_threshold;   // Index between 0 and TABLE_X_LEN
    uint32_t y_index = y_param/y_threshold;   // Index between 0 and TABLE_Y_LEN
    uint32_t x_interpol = x_param - (x_index * x_threshold);
    uint32_t y_interpol = y_param - (y_index * y_threshold);

    for (int i = 0 ; i < TABLE_SAMPLE ; i++)
    {
        uint32_t sample = 0;

        sample += (uint32_t)(table[x_index][y_index][i]) * (x_threshold-x_interpol) * (y_threshold-y_interpol);
        if (x_index < TABLE_X_LEN - 1)
        {
            sample += (uint32_t)(table[x_index+1][y_index][i]) * x_interpol * (y_threshold-y_interpol);
        }
        if (y_index < TABLE_Y_LEN - 1)
        {
            sample += (uint32_t)(table[x_index][y_index+1][i]) * (x_threshold-x_interpol) * y_interpol;
        }
        if ((x_index < TABLE_X_LEN - 1) && (y_index < TABLE_Y_LEN - 1))
        {
            sample += (uint32_t)(table[x_index+1][y_index+1][i]) * x_interpol * y_interpol;
        }
        sample /= (x_threshold * y_threshold);

        interpolated_table[i] = sample;
    }

    // Transfered from 0 to DAC_BUFFER_SIZE/2 - 1
    for (int it = 0 ; it < dac_buffer_size ; it++)
    {
        // Accumulate the phase
        h_dsp->phase += h_dsp->frequency / h_dsp->sample_frequency;
        if (h_dsp->phase >= 1.f) h_dsp->phase -= 1.f;

        uint16_t index = (uint16_t)(h_dsp->phase * TABLE_SAMPLE);

        // TODO read a wavetable instead
        dac_buffer[it] = interpolated_table[index];
    }
}