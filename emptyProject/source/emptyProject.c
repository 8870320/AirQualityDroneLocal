/*
* Licensee agrees that the example code provided to Licensee has been developed and released by Bosch solely as an example to be used as a potential reference for Licensee�s application development. 
* Fitness and suitability of the example code for any use within Licensee�s applications need to be verified by Licensee on its own authority by taking appropriate state of the art actions and measures (e.g. by means of quality assurance measures).
* Licensee shall be responsible for conducting the development of its applications as well as integration of parts of the example code into such applications, taking into account the state of the art of technology and any statutory regulations and provisions applicable for such applications. Compliance with the functional system requirements and testing there of (including validation of information/data security aspects and functional safety) and release shall be solely incumbent upon Licensee. 
* For the avoidance of doubt, Licensee shall be responsible and fully liable for the applications and any distribution of such applications into the market.
* 
* 
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are 
* met:
* 
*     (1) Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer. 
* 
*     (2) Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.  
*     
*     (3)The name of the author may not be used to
*     endorse or promote products derived from this software without
*     specific prior written permission.
* 
*  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR 
*  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
*  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
*  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
*  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
*  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
*  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
*  POSSIBILITY OF SUCH DAMAGE.
*/
/*----------------------------------------------------------------------------*/
/**
* @ingroup APPS_LIST
*
* @defgroup XDK_APPLICATION_TEMPLATE XDK Application Template
* @{
*
* @brief XDK Application Template
*
* @details Empty XDK Application Template without any functionality. Should be used as a template to start new projects.
*
* @file
**/
/* module includes ********************************************************** */

/* system header files */
#include <stdio.h>
/* additional interface header files */
#include "FreeRTOS.h"
#include "timers.h"

/* own header files */
#include "emptyProject.h"
#include "BCDS_CmdProcessor.h"
#include "BCDS_Assert.h"

#include "BCDS_Basics.h"
#include "em_cmu.h"
#include "em_adc.h"

#include "BCDS_MCU_GPIO.h"
#include "BCDS_MCU_GPIO_Handle.h"
#include "BSP_BoardShared.h"

#include "timers.h"


/* constant definitions ***************************************************** */

/* local variables ********************************************************** */

/* global variables ********************************************************* */

/* inline functions ********************************************************* */

/* local functions ********************************************************** */

/* global functions ********************************************************* */

/* constant definitions ***************************************************** */
#define TIMER_AUTORELOAD_ON pdTRUE

#define ONESECONDDELAY      UINT32_C(1000)       /* one second is represented by this macro */
#define TIMERBLOCKTIME      UINT32_C(0xffff)     /* Macro used to define blocktime of a timer */
#define TIMEBASE            UINT32_C(0)          /* Macro used to define the Timebase for the ADC */
#define ADCFREQUENCY        UINT32_C(9000000)    /* Macro Used to define the frequency of the ADC Clock */
#define NUMBER_OF_CHANNELS  UINT32_C(2)

/* local variables ********************************************************** */


void extensionLedTask(void)
{
	MCU_GPIO_Handle_T GPIOA;
	MCU_GPIO_Handle_T GPIOB;
	MCU_GPIO_Handle_T GPIOC;
    /* initialize local variables */
    GPIOA.Port = gpioPortA;
    GPIOA.Pin = 1;
    GPIOA.Mode = gpioModePushPull;
    GPIOA.InitialState = MCU_GPIO_PIN_STATE_LOW;

    GPIOB.Port = gpioPortE;
	GPIOB.Pin = 2;
	GPIOB.Mode = gpioModePushPull;
	GPIOB.InitialState = MCU_GPIO_PIN_STATE_LOW;

	GPIOC.Port = gpioPortC;
	GPIOC.Pin = 8;
	GPIOC.Mode = gpioModePushPull;
	GPIOC.InitialState = MCU_GPIO_PIN_STATE_LOW;

    /* Initialization activities for PTD driver */
    MCU_GPIO_Initialize(&GPIOA);
    MCU_GPIO_Initialize(&GPIOB);
    MCU_GPIO_Initialize(&GPIOC);
    /* blinking functionality */
    for (;;)
	{
    	int retcode;

    	vTaskDelay (5000);
		retcode = MCU_GPIO_WritePin(&GPIOA,MCU_GPIO_PIN_STATE_LOW);
		retcode = MCU_GPIO_WritePin(&GPIOB,MCU_GPIO_PIN_STATE_LOW);
		retcode = MCU_GPIO_WritePin(&GPIOC,MCU_GPIO_PIN_STATE_LOW);
		if(retcode!= RETCODE_OK){
			printf("Write low failed!\n\r\n\r");
		}
		else{
			printf("0.\n\r");
		}

		vTaskDelay (5000);
		retcode = MCU_GPIO_WritePin(&GPIOA,MCU_GPIO_PIN_STATE_HIGH);
		retcode = MCU_GPIO_WritePin(&GPIOB,MCU_GPIO_PIN_STATE_LOW);
		retcode = MCU_GPIO_WritePin(&GPIOC,MCU_GPIO_PIN_STATE_LOW);
		if(retcode!= RETCODE_OK){
			printf("Write low failed!\n\r\n\r");
		}
		else{
			printf("1.\n\r");
		}

		vTaskDelay (5000);
		retcode = MCU_GPIO_WritePin(&GPIOA,MCU_GPIO_PIN_STATE_LOW);
		retcode = MCU_GPIO_WritePin(&GPIOB,MCU_GPIO_PIN_STATE_HIGH);
		retcode = MCU_GPIO_WritePin(&GPIOC,MCU_GPIO_PIN_STATE_LOW);
		if(retcode!= RETCODE_OK){
			printf("Write low failed!\n\r\n\r");
		}
		else{
			printf("2.\n\r");
		}

		vTaskDelay (5000);
		retcode = MCU_GPIO_WritePin(&GPIOA,MCU_GPIO_PIN_STATE_HIGH);
		retcode = MCU_GPIO_WritePin(&GPIOB,MCU_GPIO_PIN_STATE_HIGH);
		retcode = MCU_GPIO_WritePin(&GPIOC,MCU_GPIO_PIN_STATE_LOW);
		if(retcode!= RETCODE_OK){
			printf("Write low failed!\n\r\n\r");
		}
		else{
			printf("3.\n\r");
		}

		vTaskDelay (5000);
		retcode = MCU_GPIO_WritePin(&GPIOA,MCU_GPIO_PIN_STATE_LOW);
		retcode = MCU_GPIO_WritePin(&GPIOB,MCU_GPIO_PIN_STATE_LOW);
		retcode = MCU_GPIO_WritePin(&GPIOC,MCU_GPIO_PIN_STATE_HIGH);
		if(retcode!= RETCODE_OK){
			printf("Write low failed!\n\r\n\r");
		}
		else{
			printf("4.\n\r");
		}

		vTaskDelay (5000);
		retcode = MCU_GPIO_WritePin(&GPIOA,MCU_GPIO_PIN_STATE_HIGH);
		retcode = MCU_GPIO_WritePin(&GPIOB,MCU_GPIO_PIN_STATE_LOW);
		retcode = MCU_GPIO_WritePin(&GPIOC,MCU_GPIO_PIN_STATE_HIGH);
		if(retcode!= RETCODE_OK){
			printf("Write low failed!\n\r\n\r");
		}
		else{
			printf("5.\n\r");
		}

		vTaskDelay (5000);
		retcode = MCU_GPIO_WritePin(&GPIOA,MCU_GPIO_PIN_STATE_LOW);
		retcode = MCU_GPIO_WritePin(&GPIOB,MCU_GPIO_PIN_STATE_HIGH);
		retcode = MCU_GPIO_WritePin(&GPIOC,MCU_GPIO_PIN_STATE_HIGH);
		if(retcode!= RETCODE_OK){
			printf("Write low failed!\n\r\n\r");
		}
		else{
			printf("6.\n\r");
		}

		vTaskDelay (5000);
		retcode = MCU_GPIO_WritePin(&GPIOA,MCU_GPIO_PIN_STATE_HIGH);
		retcode = MCU_GPIO_WritePin(&GPIOB,MCU_GPIO_PIN_STATE_HIGH);
		retcode = MCU_GPIO_WritePin(&GPIOC,MCU_GPIO_PIN_STATE_HIGH);
		if(retcode!= RETCODE_OK){
			printf("Write low failed!\n\r\n\r");
		}
		else{
			printf("7.\n\r");
		}

//		vTaskDelay (1000);
//		retcode = MCU_GPIO_ReadPin(&GPIOB,inputGPIO2);
//				if(retcode!= RETCODE_OK){
//							printf("Read failed!\n\r\n\r");
//						}
//		printf("Level at GPIO2:%d\n\r", *inputGPIO2);



    }
}



void createNewGPIOTask(void)
{
  xTaskHandle taskHandle = NULL;
  xTaskCreate(
    extensionLedTask,                 // function that implements the task
    (const char * const) "My Task", // a name for the task
    configMINIMAL_STACK_SIZE,       // depth of the task stack
    NULL,                           // parameters passed to the function
    tskIDLE_PRIORITY,               // task priority
    taskHandle                      // pointer to a task handle for late reference
  );
}



xTimerHandle scanAdcTimerHandle;

/**
 * @brief Timer Callback; Scans and prints out the ADC Channels' Values
 */
static void scanAdc(xTimerHandle pxTimer)
{
    /* Initialize the Variables */
    (void) (pxTimer);
    uint32_t _adc0ChData = 0;
    uint8_t _channelsScanned = 0;

    /* Start the ADC Scan */
    ADC_Start(ADC0, adcStartScan);

    for (_channelsScanned = 0; _channelsScanned < NUMBER_OF_CHANNELS-1; _channelsScanned++) {
        /* Wait for Valid Data */
        while (!(ADC0->STATUS & ADC_STATUS_SCANDV));

        /* Read the Scanned data */
        _adc0ChData = 0xFFF & ADC_DataScanGet(ADC0);
        printf("ADC Channel %ld = %ld\r\n", ((ADC0->STATUS & _ADC_STATUS_SCANDATASRC_MASK) >> _ADC_STATUS_SCANDATASRC_SHIFT), _adc0ChData);
    }
}

/**
 * @brief Configure the ADC to scan multiple channels;
 *        Initialize and Start a time to scan the ADC
 */
static void scanAdcInit(void)
{
    /* Initialize Configuration ADC Structures */
    ADC_Init_TypeDef     adc0_init_conf     = ADC_INIT_DEFAULT;
    ADC_InitScan_TypeDef adc0_scaninit_conf = ADC_INITSCAN_DEFAULT;

    /* Enable Clocks for the ADC */
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_ADC0, true);

    /* Configure the ADC Initialization Structure */
    adc0_init_conf.timebase = ADC_TimebaseCalc(TIMEBASE);
    adc0_init_conf.prescale = ADC_PrescaleCalc(ADCFREQUENCY, TIMEBASE);
    ADC_Init(ADC0, &adc0_init_conf);

    /* Configure the ADC Scan Structure */
    adc0_scaninit_conf.reference = adcRef2V5;
    adc0_scaninit_conf.input     = ADC_SCANCTRL_INPUTMASK_CH5 |
                                   ADC_SCANCTRL_INPUTMASK_CH6;

    ADC_InitScan(ADC0, &adc0_scaninit_conf);

    /* Setup and Start the timer to scan the ADC Channels */
    scanAdcTimerHandle = xTimerCreate(
        (const char *) "ADC read", ONESECONDDELAY,
        TIMER_AUTORELOAD_ON, NULL, scanAdc);

    xTimerStart(scanAdcTimerHandle, TIMERBLOCKTIME);
}



/**
 * @brief This is a template function where the user can write his custom application.
 *
 */
void appInitSystem(void * CmdProcessorHandle, uint32_t param2)
{
    if (CmdProcessorHandle == NULL)
    {
        printf("Command processor handle is null \n\r");
        assert(false);
    }
    BCDS_UNUSED(param2);
    Board_EnablePowerSupply3V3(EXTENSION_BOARD);
    scanAdcInit();
    createNewGPIOTask();
}
/**@} */
/** ************************************************************************* */
