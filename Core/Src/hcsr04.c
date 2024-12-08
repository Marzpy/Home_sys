#include "main.h"
#include "tim.h"

#include "hcsr04.h"

#define HCSR04_CALCULATION_CONST  ((float)0.01715)

HCSR04_Status_t HCSR04_GetDistanceFloat(HCSR04_t *hcsr04, float *Result)
{
	*Result = (float)hcsr04->Result_us * HCSR04_CALCULATION_CONST;

	  return HCSR04_OK;
}

HCSR04_Status_t HCSR04_GetDistanceInteger(HCSR04_t *hcsr04, uint16_t *Result)
{
	*Result = hcsr04->Result_us / 58;

	  return HCSR04_OK;
}

void HCSR04_InterruptHandler(HCSR04_t *hcsr04)
{
	hcsr04->Result_us = __HAL_TIM_GET_COMPARE(hcsr04->htim_echo, hcsr04->EchoTimChannelStop) - __HAL_TIM_GET_COMPARE(hcsr04->htim_echo, hcsr04->EchoTimChannelStart);
}


HCSR04_Status_t HCSR04_Init(HCSR04_t *hcsr04, TIM_HandleTypeDef *timer_trigger, TIM_HandleTypeDef *timer_echo, uint32_t TriggerTimChannel, uint32_t EchoTimChannelStart, uint32_t EchoTimChannelStop)
{
	hcsr04->htim_trigger = timer_trigger;
	hcsr04->htim_echo = timer_echo;

	hcsr04->TriggerTimChannel = TriggerTimChannel;

	hcsr04->EchoTimChannelStart = EchoTimChannelStart;
	hcsr04->EchoTimChannelStop = EchoTimChannelStop;

	HAL_TIM_Base_Start(hcsr04->htim_echo);
	HAL_TIM_IC_Start(hcsr04->htim_echo, hcsr04->EchoTimChannelStart);
	HAL_TIM_IC_Start_IT(hcsr04->htim_echo, hcsr04->EchoTimChannelStop);

	HAL_TIM_Base_Start(hcsr04->htim_trigger);
	HAL_TIM_PWM_Start(hcsr04->htim_trigger, hcsr04->TriggerTimChannel);

	return HCSR04_OK;
}
