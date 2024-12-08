void handle_menu(void)
{
    char lcd_buf[16];

    // Wyświetlanie menu na LCD
    switch (menu_option)
    {
        case 0:
            strncpy((char *)disp.f_line, "Menu: Temp & Hum", 16);
            strncpy((char *)disp.s_line, "Press Select", 16);
            break;
        case 1:
            strncpy((char *)disp.f_line, "Menu: Distance", 16);
            strncpy((char *)disp.s_line, "Press Select", 16);
            break;
        case 2:
            strncpy((char *)disp.f_line, "Menu: Option 3", 16);
            strncpy((char *)disp.s_line, "Press Select", 16);
            break;
    }
    lcd_display(&disp);

    // Obsługa przycisków
    if (!HAL_GPIO_ReadPin(BUTTON_NEXT_PORT, BUTTON_NEXT_PIN))  // Sprawdzanie stanu przycisku NEXT
    {
        HAL_Delay(200);  // Debouncing
        menu_option = (menu_option + 1) % menu_options_count;  // Zwiększ opcję i zawiń do 0
    }
    else if (!HAL_GPIO_ReadPin(BUTTON_SELECT_PORT, BUTTON_SELECT_PIN))  // Przycisk SELECT
    {
        HAL_Delay(200);  // Debouncing
        switch (menu_option)
        {
            case 0:
                // Wyświetlanie danych DHT11
                DHT_GetData(&DHT11_Data);
                sprintf(lcd_buf, "Temp: %.1f C", DHT11_Data.Temperature);
                strncpy((char *)disp.f_line, lcd_buf, 16);

                sprintf(lcd_buf, "Hum: %.1f %%", DHT11_Data.Humidity);
                strncpy((char *)disp.s_line, lcd_buf, 16);
                break;
            case 1:
                // Wyświetlanie danych HC-SR04
                HCSR04_GetDistanceFloat(&HCSR04, &Distance_f);
                sprintf(lcd_buf, "Dist: %.2f cm", Distance_f);
                strncpy((char *)disp.f_line, lcd_buf, 16);

                strncpy((char *)disp.s_line, " ", 16);  // Pusta linia
                break;
            case 2:
                // Opcja 3: Możesz dodać inne czujniki lub dane
                strncpy((char *)disp.f_line, "Option 3 data", 16);
                strncpy((char *)disp.s_line, "Unavailable", 16);
                break;
        }
        lcd_display(&disp);
        HAL_Delay(1000);  // Wyświetlenie danych przez chwilę
    }
}
