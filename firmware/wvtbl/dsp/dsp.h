#ifndef __DSP_H
#define __DSP_H

#include <stdint.h>

#define FREQ_C4 523.251f

typedef enum dsp_param_id_enum
{
    PITCH_PARAM,
    X_PARAM,
    Y_PARAM,
    PARAMS_LEN
} dsp_param_id_t;

typedef enum dsp_input_id_enum
{
    PITCH_INPUT,
    X_INPUT,
    Y_INPUT,
    INPUTS_LEN
} dsp_input_id_t;

typedef enum dsp_output_id_enum
{
    AUDIO_OUTPUT,
    OUTPUTS_LEN
} dsp_output_id_t;

typedef enum dsp_light_id_enum
{
    RED_LIGHT,
    GREEN_LIGHT,
    BLUE_LIGHT,
    LIGHTS_LEN
} dsp_light_id_t;

typedef struct h_dsp_struct
{
    float params[PARAMS_LEN];
    float inputs[INPUTS_LEN];
    float outputs[OUTPUTS_LEN];
    float lights[LIGHTS_LEN];

    float sample_frequency;

    float phase;
} h_dsp_t;

void dsp_init(h_dsp_t * h_dsp);
void dsp_process(h_dsp_t * h_dsp);

#endif // __DSP_H