/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "i2c_lcd.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "dht.h"
#include "hcsr04.h"
#include "menu_lcd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define HCSR04_TRIGGER_TIMER		htim3
#define HCSR04_ECHO_TIMER			htim3
#define HCSR04_TRIGGER_CHANNEL		TIM_CHANNEL_3
#define HCSR04_ECHO_START_CHANNEL 	TIM_CHANNEL_1
#define HCSR04_ECHO_STOP_CHANNEL 	TIM_CHANNEL_2

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint8_t motionStartTick = 0; //czas rozpoczęcia PIR
volatile uint32_t motionDuration = 0; //czas trwania PIR
volatile uint8_t motionInProgress = 0; //Flaga w toku
 struct lcd_disp disp;
 char uart_buf2[50];
char lcd_buf[16];
 HCSR04_t HCSR04;
float Distance_f;
uint16_t Distance_u16;
FRESULT FatFsResult;
FATFS SdFatFs;
FIL SdCardFile;

uint8_t bytes;
char data[128];


float global_temperature = 0.0f;
float global_humidity = 0.0f;
float global_distance = 0.0f;
float global_time_move= 0.0f;

//struct lcd_disp lcd_display;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
DHT_DataTypedef DHT11_Data;
float Temperature, Humidity;
MenuState currentMenuState = MENU_MAIN;
//Menu_HandleInput(); 
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
   
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */


  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();
  MX_SPI2_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
  
  HCSR04_Init(&HCSR04, &HCSR04_TRIGGER_TIMER, &HCSR04_ECHO_TIMER, HCSR04_TRIGGER_CHANNEL, HCSR04_ECHO_START_CHANNEL, HCSR04_ECHO_STOP_CHANNEL);
 

  disp.addr = (0x27 << 1);
  disp.bl = true;
  lcd_init(&disp);
  Menu_Display(currentMenuState);

  //Log_Data_To_SD()''

  // sprintf((char *)disp.f_line, " Uruchamianie ");
   // sprintf((char *)disp.s_line, " Wyświetlacza");
   //lcd_display(&disp);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

     HCSR04_GetDistanceFloat(&HCSR04, &Distance_f);
	  HCSR04_GetDistanceInteger(&HCSR04, &Distance_u16);
	  HAL_Delay(10);

DHT_GetData(&DHT11_Data);
  	  Temperature = DHT11_Data.Temperature;
	  Humidity = DHT11_Data.Humidity;

global_distance=HCSR04_GetDistanceFloat(&HCSR04, &Distance_f);
global_humidity=Humidity;
global_temperature=Temperature;  

sprintf(uart_buf2, "dyst: %.2f \r\n", global_distance);

// Przesłanie przez UART
HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf2, strlen(uart_buf2), 10);

	 // HAL_Delay(3000);
 Menu_HandleInput();
Menu_Display(currentMenuState);
      



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_TIM34;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.Tim34ClockSelection = RCC_TIM34CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim == HCSR04.htim_echo)
	{
		HCSR04_InterruptHandler(&HCSR04);
	}
}





void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == PIR_Pin) {  // Jeśli przerwanie pochodzi od czujnika PIR
        if (HAL_GPIO_ReadPin(PIR_GPIO_Port, PIR_Pin) == GPIO_PIN_SET) {
            if (!motionInProgress) { // Ruch się rozpoczął
                motionStartTick = HAL_GetTick();
                motionInProgress = 1; 
                HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin,GPIO_PIN_SET); // Ustawienie flagi
            }
        } else if (motionInProgress) { // Ruch zakończony
            motionDuration = HAL_GetTick() - motionStartTick;
            motionInProgress = 0;  // Zresetowanie flagi
            // Możesz dodać kod do przetwarzania motionDuration, np. zapis na kartę SD
        }
    }
}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
