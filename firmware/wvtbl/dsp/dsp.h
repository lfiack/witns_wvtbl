/**
 * @file dsp.h
 * @brief This is a header file for a DSP library.
 */

#ifndef __DSP_H
#define __DSP_H

#include <stdint.h>

/**
 * @brief The frequency of middle C (C4).
 */
#define FREQ_C4 523.251f

/**
 * @brief Enumeration for identifying different parameters in the DSP structure. 
 * X and Y parameters are modified by the potentiometers and the pitch is modified by the encoder
 */
typedef enum dsp_param_id_enum
{
    PITCH_PARAM, /**< Identifier for the pitch parameter. */
    X_PARAM, /**< Identifier for the X parameter. */
    Y_PARAM, /**< Identifier for the Y parameter. */
    PARAMS_LEN /**< Length of the parameter array. */
} dsp_param_id_t;

/**
 * @brief Enumeration for identifying different inputs.
 * Pitch, X and Y come from jack inputs
 */
typedef enum dsp_input_id_enum
{
    PITCH_INPUT, /**< Identifier for the pitch input. */
    X_INPUT, /**< Identifier for the X input. */
    Y_INPUT, /**< Identifier for the Y input. */
    INPUTS_LEN /**< Length of the input array. */
} dsp_input_id_t;

/**
 * @brief Enumeration for identifying different outputs.
 */
typedef enum dsp_output_id_enum
{
    AUDIO_OUTPUT, /**< Identifier for the audio output. */
    OUTPUTS_LEN /**< Length of the output array. */
} dsp_output_id_t;

/**
 * @brief Enumeration for identifying the encoder's LED.
 */
typedef enum dsp_light_id_enum
{
    RED_LIGHT, /**< Identifier for the red LED. */
    GREEN_LIGHT, /**< Identifier for the green LED. */
    BLUE_LIGHT, /**< Identifier for the blue LED. */
    LIGHTS_LEN /**< Length of the lights array. */
} dsp_light_id_t;

/**
 * @brief Structure for storing digital signal processing information.
 */
typedef struct h_dsp_struct
{
    float params[PARAMS_LEN]; /**< Array for storing parameters. */
    float inputs[INPUTS_LEN]; /**< Array for storing inputs. */
    float outputs[OUTPUTS_LEN]; /**< Array for storing outputs. */
    float lights[LIGHTS_LEN]; /**< Array for storing lights. */

    float sample_frequency; /**< The sample frequency of the digital signal. */

    float phase; /**< The phase of the digital signal. */
} h_dsp_t;

/**
 * @brief Initializes the DSP structure.
 * @param h_dsp Pointer to the DSP structure to be initialized.
 */
void dsp_init(h_dsp_t * h_dsp);

/**
 * @brief Processes the digital signal.
 * @param h_dsp Pointer to the DSP structure for processing the digital signal.
 */
void dsp_process(h_dsp_t * h_dsp);

#endif // __DSP_H
