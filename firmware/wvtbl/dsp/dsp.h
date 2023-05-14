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

#define PARAM_MIN 0
#define PARAM_MAX 4095

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
 * @brief Structure for storing digital signal processing information.
 */
typedef struct h_dsp_struct
{
    int32_t params[PARAMS_LEN]; /**< Array for storing parameters. */
    int32_t inputs[INPUTS_LEN]; /**< Array for storing inputs. */

    float sample_frequency; /**< The sample frequency of the digital signal. */
    float frequency;

    float phase; /**< The phase of the digital signal. */
} h_dsp_t;

float fclamp (float voltage, float min, float max);

int32_t clamp (int32_t voltage, int32_t min, int32_t max);

/**
 * @brief Initializes the DSP structure.
 * @param h_dsp Pointer to the DSP structure to be initialized.
 */
void dsp_init(h_dsp_t * h_dsp);

/**
 * @brief Processes the digital signal.
 * @param h_dsp Pointer to the DSP structure for processing the digital signal.
 */
void dsp_process(h_dsp_t * h_dsp, uint32_t * dac_buffer, int32_t dac_buffer_size);

#endif // __DSP_H
