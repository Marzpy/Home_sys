#ifndef INC_HCSR04_H_
#define INC_HCSR04_H_

#define HCSR04_CALCULATION_COST ((float)0.01715)

typedef enum
{
	HCSR04_OK, 		// 0
	HCSR04_ERROR 	// 1
} HCSR04_Status_t;

typedef struct
{
	TIM_HandleTypeDef 	*htim_trigger;
	TIM_HandleTypeDef 	*htim_echo;

	uint32_t 			TriggerTimChannel;

	uint32_t 			EchoTimChannelStart;
	uint32_t 			EchoTimChannelStop;

	uint16_t			Result_us;

} HCSR04_t;

void HCSR04_InterruptHandler(HCSR04_t *hcsr04);

HCSR04_Status_t HCSR04_Init(HCSR04_t *hcsr04, TIM_HandleTypeDef *timer_trigger, TIM_HandleTypeDef *timer_echo, uint32_t TriggerTimChannel, uint32_t EchoTimChannelStart, uint32_t EchoTimChannelStop);

HCSR04_Status_t HCSR04_GetDistanceFloat(HCSR04_t *hcsr04, float *Result);
HCSR04_Status_t HCSR04_GetDistanceInteger(HCSR04_t *hcsr04, uint16_t *Result);

#endif /* INC_HCSR04_H_ */