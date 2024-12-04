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
float global_temperature = 0.0f;
float global_humidity = 0.0f;
float global_distance = 0.0f;
//MenuState currentMenuState = MENU_MAIN;

extern HCSR04_t HCSR04;
//struct lcd_disp disp2;
extern struct lcd_disp disp;

char lcd_buf3[16];


void Menu_Display(MenuState state) {
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

        default:
            strncpy((char *)disp.f_line, "Unknown State", 16);
            strncpy((char *)disp.s_line, " ", 16);
            break;
    }

    lcd_display(&disp);  // Wyświetlenie na ekranie
}
void Menu_HandleInput(void) {
    if (HAL_GPIO_ReadPin(button1_GPIO_Port, button1_Pin) == GPIO_PIN_SET) {
        // currentMenuState = (currentMenuState + 1) % 3;  // Przełącz na kolejny stan
        currentMenuState = MENU_SENSOR_1;
        Menu_Display(currentMenuState);
        HAL_Delay(200);  // Debouncing
    }

    if (HAL_GPIO_ReadPin(button2_GPIO_Port, button2_Pin) == GPIO_PIN_SET) {
        //currentMenuState = (currentMenuState == 0) ? 2 : currentMenuState - 1;  // Powrót do poprzedniego stanu
        currentMenuState = MENU_SENSOR_2;
        
        Menu_Display(currentMenuState);
        HAL_Delay(200);  // Debouncing
    }
}