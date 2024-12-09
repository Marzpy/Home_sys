#include "main.h"
#include "fatfs.h"
#include <stdio.h>
#include <string.h>

// Globalne zmienne
extern float global_temperature = 0.0f;
extern float global_distance = 0.0f;
extern int motion_detected = 0;

// Timery
uint32_t last_temp_log = 0;
uint32_t last_hcsr_log = 0;

void Log_Data_To_SD(void) {
    FRESULT res;
    FIL file;
    char buffer[64];

    // Montowanie systemu plików
    if (f_mount(&SdFatFs, "", 1) == FR_OK) {
        // Otwórz plik do zapisu lub stwórz, jeśli nie istnieje
        res = f_open(&file, "datalog.txt", FA_WRITE | FA_OPEN_APPEND);
        if (res == FR_OK) {
            uint32_t current_time = HAL_GetTick();

            // Zapis temperatury co 30 sekund
            if (current_time - last_temp_log >= 30000) {
                last_temp_log = current_time;
                snprintf(buffer, sizeof(buffer), "Temperature: %.2f C\n", global_temperature);
                f_printf(&file, buffer);
            }

            // Zapis odległości i PIR co 5 sekund
            if (current_time - last_hcsr_log >= 5000) {
                last_hcsr_log = current_time;
                snprintf(buffer, sizeof(buffer), "Distance: %.2f cm, Motion: %d\n", global_distance, motion_detected);
                f_printf(&file, buffer);
            }

            // Zamknij plik po zapisie
            f_close(&file);
        }
        f_mount(NULL, "", 1); // Odmontowanie systemu plików
    }
}