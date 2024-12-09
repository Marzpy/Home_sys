#ifndef MENU_H
#define MENU_H

#include "i2c_lcd.h"
#include "stdint.h"

typedef enum{
    MENU_MAIN,
    MENU_SENSOR_1,
    MENU_SENSOR_2,
    MENU_SENSOR_3,
}MenuState;
extern MenuState currentMenuState; 
void Menu_Display(MenuState state);
void Menu_HandleInput(void);
#endif