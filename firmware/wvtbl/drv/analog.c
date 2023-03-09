#include "analog.h"

int8_t analog_init(h_analog_t * h_analog)
{
    for (int it = 0 ; it < ANALOG_ADC_NUMBER ; it++)
    {
        if (HAL_OK != HAL_ADCEx_Calibration_Start(h_analog->adc_handles[it], ADC_SINGLE_ENDED))
        {
            return -1;
        }
    }

    if (HAL_OK != HAL_ADCEx_MultiModeStart_DMA(h_analog->adc_handles[0], (uint32_t *)h_analog->adc_buffer, ANALOG_ADC_BUFFER_LENGTH_WORDS)) 
    {
        return -1;
    }

    if (HAL_OK != HAL_DAC_Start(h_analog->dac_handle, h_analog->dac_channel))
    {
        return -1;
    }

    if (HAL_OK != HAL_TIM_Base_Start(h_analog->timer_handle))
    {
        return -1;
    }

    return 0;
}

uint16_t analog_get_adc(h_analog_t * h_analog, analog_adc_index_t index)
{
    return h_analog->adc_buffer[index];
    // Nothing else to do, everything is done by DMA
}

void analog_set_dac(h_analog_t * h_analog, uint16_t value)
{
    h_analog->dac_value = value;
    HAL_DAC_SetValue(h_analog->dac_handle, h_analog->dac_channel, DAC_ALIGN_12B_R, value);
}