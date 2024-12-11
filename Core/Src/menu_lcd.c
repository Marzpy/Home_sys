#include "main.h"
#include "menu_lcd.h"
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "dht.h"
#include "hcsr04.h"
#include "i2c_lcd.h"
#include "stm32f3xx_hal.h"
// Globalne zmienne na wyniki
extern float global_temperature;
extern float global_humidity;
extern float global_distance;
extern float global_time_move;
//MenuState currentMenuState = MENU_MAIN;

extern HCSR04_t HCSR04;
//extern motion_duration;

//struct lcd_disp disp2;
extern struct lcd_disp disp;

char lcd_buf3[16];


void Menu_Display(MenuState state) {
    static uint32_t last_button1_press = 0;
    static uint32_t last_button2_press = 0;
     // Bufor dla linii wyświetlacza
//strncpy((char *)disp.f_line, "Main Menu", 16);
          //  strncpy((char *)disp.s_line, "Select Option", 16);
    switch (state) {
        case MENU_MAIN:
            strncpy((char *)disp.f_line, "Main Menu", 16);
            strncpy((char *)disp.s_line, "Select Option", 16);
            break;

        case MENU_SENSOR_1:
            sprintf(lcd_buf3, "Temp: %.1f C", global_temperature);
            strncpy((char *)disp.f_line, lcd_buf3, 16);

            sprintf(lcd_buf3, "Hum: %.1f %%", global_humidity);
            strncpy((char *)disp.s_line, lcd_buf3, 16);
            break;

        case MENU_SENSOR_2:
            sprintf(lcd_buf3, "Dist: %.2f cm", global_distance);
            strncpy((char *)disp.f_line, lcd_buf3, 16);

            strncpy((char *)disp.s_line, " ", 16); // Pusta linia
            break;

         case MENU_SENSOR_3:
            sprintf(lcd_buf3, "Time: %.2f s", global_time_move);
            strncpy((char *)disp.f_line, lcd_buf3, 16);

            strncpy((char *)disp.s_line, " ", 16); // Pusta linia
            break;

        default:
            sprintf(lcd_buf3, "Menu:", global_time_move);
            strncpy((char *)disp.f_line, lcd_buf3, 16);

            strncpy((char *)disp.s_line, " ", 16);
            break;
    }

    lcd_display(&disp); 
    HAL_Delay(1000);  // Wyświetlenie na ekranie
}
void Menu_HandleInput(void) {
    if (HAL_GPIO_ReadPin(button1_GPIO_Port, button1_Pin) == GPIO_PIN_RESET) {
        // currentMenuState = (currentMenuState + 1) % 3;  // Przełącz na kolejny stan
        currentMenuState = MENU_SENSOR_1;
        Menu_Display(currentMenuState);
        HAL_Delay(300);  // Debouncing
    }

    if (HAL_GPIO_ReadPin(button2_GPIO_Port, button2_Pin) == GPIO_PIN_RESET) {
        //currentMenuState = (currentMenuState == 0) ? 2 : currentMenuState - 1;  // Powrót do poprzedniego stanu
        currentMenuState = MENU_SENSOR_2;
        
        Menu_Display(currentMenuState);
        HAL_Delay(300);  // Debouncing
    }
     if ((HAL_GPIO_ReadPin(button1_GPIO_Port, button1_Pin) == GPIO_PIN_RESET) && (HAL_GPIO_ReadPin(button2_GPIO_Port, button2_Pin) == GPIO_PIN_RESET)) {
        // currentMenuState = (currentMenuState + 1) % 3;  // Przełącz na kolejny stan
        currentMenuState = MENU_SENSOR_3;
        Menu_Display(currentMenuState);
        HAL_Delay(300);  // Debouncing
    }
}
