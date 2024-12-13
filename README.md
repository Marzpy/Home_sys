Project: Multi-Sensor Monitoring System with SD Card Data Logging

This project is a multi-sensor monitoring system based on an STM32 microcontroller. 
It measures temperature, humidity, distance, and motion.
The results are displayed on an LCD screen and logged to an SD card for later analysis. 
The system includes a simple menu interface controlled by two buttons.

Features
Environmental Data Measurement:
Temperature and Humidity (DHT11) — logged every 30 seconds.
Distance (HCSR04) — logged every 5 seconds.
Motion (PIR sensor) — state checked every 5 seconds.
LCD Display:
Displays data in text format.
Simple menu navigation using two buttons.
SD Card Logging:
Data is saved to a file datalog.txt using the FatFS library.
User Interface:
Main menu with options to view sensor data.
Data is logged at regular intervals to the SD card.

Components Used
STM32: Microcontroller to handle the project logic.
DHT11: Temperature and humidity sensor.
HCSR04: Ultrasonic sensor for distance measurement.
PIR: Motion sensor.
LCD 16x2: Display module for presenting data.
SD Card: For logging data.
Buttons: For menu navigation.
