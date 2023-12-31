/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "driver/gpio.h"

#define BUTTON_PIN GPIO_NUM_13
#define PRESSED_STATE 0
#define NORMAL_STATE 1

int button1_flag = 0;
int KeyReg3 = NORMAL_STATE;
int KeyReg2 = NORMAL_STATE;
int KeyReg1 = NORMAL_STATE;
int KeyReg0 = NORMAL_STATE;

// process when know that button is pressed.
void subKeyProcess(){
	button1_flag = 1;
    printf("ESP32\n");
	if(KeyReg3 == PRESSED_STATE){
		if ( button1_flag == 0){
			button1_flag = 1- button1_flag;
		}else{
			button1_flag = 1- button1_flag;
		}
	}
}

// cyclic task which print student identifier every second.
void studentID1(void * p)
{
    for (;;)
    {
        printf("1234567\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}


void pollingButton(void * p)
{
	// set pin to be pullup input only
    esp_rom_gpio_pad_select_gpio ( BUTTON_PIN ) ;
    gpio_set_direction ( BUTTON_PIN , GPIO_MODE_INPUT ) ;
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);


// loop that receive signal from input pin above then give it to process function
    for (;;)
    {
	    // debounce button signal
        KeyReg2 = KeyReg1;
        KeyReg1 = KeyReg0;
        KeyReg0 = gpio_get_level(BUTTON_PIN);
        if ((KeyReg1 == KeyReg0) && (KeyReg1 == KeyReg2)){
            if (KeyReg2 != KeyReg3)
            {
                KeyReg3 = KeyReg2;

                if (KeyReg3 == PRESSED_STATE){
			// when button is pressed
                    subKeyProcess();
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    vTaskDelete(NULL);
}

void app_main(void)
{
	// first task create
    xTaskCreate(&studentID1, "ID1", 2048, NULL, 5, NULL);
	// second task create
    xTaskCreate(&pollingButton, "ID2", 2048, NULL, 5, NULL);
}
