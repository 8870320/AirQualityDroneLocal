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
#include "BCDS_HAL.h"
#include "FreeRTOS.h"
#include "timers.h"
//#include "BSP_UART.h"
#include "BCDS_MCU_UART.h" /*Header file containing HAL interfaces*/
#include "BSP_ExtensionPort.h"
#include "BCDS_CmdProcessor.h"
#include "BCDS_Assert.h"

CmdProcessor_T *AppCmdProcessorHandle;

UART_T uartHandle;
//
//typedef void (*MCU_UART_Callback_T)(UART_T uart, struct MCU_UART_Event_S event);
//Retcode_T MCU_UART_Initialize(UART_T uart, MCU_UART_Callback_T callback);
//Retcode_T MCU_UART_Send(UART_T uart, uint8_t * data, uint32_t len);
//Retcode_T MCU_UART_Receive(UART_T uart, uint8_t* buffer, uint32_t size);
//Retcode_T MCU_UART_GetRxCount(UART_T uart, uint32_t* count);
//
//Retcode_T BSP_ExtensionPort_ConnectUart(void);
//HWHandle_T BSP_ExtensionPort_GetUartHandle(void);
//Retcode_T BSP_ExtensionPort_EnableUart(void);
//Retcode_T BSP_ExtensionPort_SetUartConfig(BSP_ExtensionPortUartConfig_T configItem, uint32_t value, void * pvalue);


MCU_UART_Callback_T uartCallback(UART_T uart, struct MCU_UART_Event_S event){
	printf("UartCallback\r\n");
	return null;
}

void initUART(){
	int status;
	printf("Startup...\r\n");
	vTaskDelay (8000);
	printf("Startup... done\r\n");
	status= BSP_ExtensionPort_ConnectUart();
	int x =RETCODE_BSP_EXTENSIONPORT_GPIO_IN_USE;
	if(status!=RETCODE_OK){
		printf("Error at BSP_ExtensionPort_ConnectUart: %d\r\n",status);
	}
	else{
		printf("Uart connected successfully\r\n");
	}

	uartHandle = BSP_ExtensionPort_GetUartHandle();
	if(uartHandle==NULL){
		printf("UART Handle initialized wrong");
	}
	else{
		printf("UART Handle initialized correctly");
	}


	status = MCU_UART_Initialize(uartHandle,uartCallback);
	if(status!=RETCODE_OK){
		printf("Error at MCU_UART_Initialize: %d\r\n",status);
	}
	else{
		printf("MCU_UART_Initialize initialized correctly\r\n");
	}

	status= BSP_ExtensionPort_EnableUart();
	if(status!=RETCODE_OK){
		printf("Error at BSP_ExtensionPort_EnableUart: %d\r\n",status);
	}
	else{
		printf("UART enabled correctly\r\n");
	}
}

void testUART(){
	int status;
	uint8_t buffer [10] ;
	buffer[0]=0;
	buffer[1]=1;
	buffer[2]=2;
	uint32_t bufLen = sizeof(buffer);

	for (;;)
	{
		printf("Startup task...\r\n");
		vTaskDelay (5000);
		printf("Startup task... done\r\n");

		status = MCU_UART_Receive(uartHandle,  buffer, bufLen);
		if(status!=RETCODE_OK){
			printf("Error at MCU_UART_Receive: %d\r\n",status);
		}
		else{
			printf("MCU_UART_Receive worked correctly\r\n");
		}

		status = MCU_UART_Send(uartHandle,  buffer, bufLen);
		if(status!=RETCODE_OK){
			printf("Error at MCU_UART_Send: %d\r\n",status);
		}
		else{
			printf("MCU_UART_Send worked correctly\r\n");
		}

		//----------------------------------

	//	status = MCU_UART_Receive();
	//	if(status!=RETCODE_OK){
	//		printf("Error at establishing UART initializing.\r\n");
	//	}
	//
	//	status = BSP_UART_Connect();
	//	if(status!=RETCODE_SUCCESS){
	//		printf("Error at establishing UART initializing.\r\n");
	//	}
	//
	//	status = BSP_UART_Enable();
	//	if(status!=RETCODE_SUCCESS){
	//		printf("Error at establishing UART connection.\r\n");
	//	}
		vTaskDelay (5000);
	}
}

void createNewUARTTask(void)
{
  xTaskHandle taskHandle = NULL;
  xTaskCreate(
    testUART,                 // function that implements the task
    (const char * const) "My Task", // a name for the task
    configMINIMAL_STACK_SIZE,       // depth of the task stack
    NULL,                           // parameters passed to the function
    tskIDLE_PRIORITY,               // task priority
    taskHandle                      // pointer to a task handle for late reference
  );
}

void appInitSystem(void * CmdProcessorHandle, uint32_t param2)
{
    if (CmdProcessorHandle == NULL)
    {
        printf("Command processor handle is null \r\n");
        assert(false);
    }
    AppCmdProcessorHandle = (CmdProcessor_T *) CmdProcessorHandle;
    Board_EnablePowerSupply3V3(EXTENSION_BOARD);
    BCDS_UNUSED(param2);
    Retcode_T returnVal = RETCODE_OK;

    initUART();
    createNewUARTTask();
//    returnVal = MCU_UART_Initialize(processdata1);
//    if (returnVal == RETCODE_OK)
//    {
//        returnVal = HAL_UART_Enable_Rx();
//    }
//    if (returnVal != RETCODE_OK)
//    {
//        printf("Enabling the UART for receiving failed \r\n");
//    }
}




/**@} */
/** ************************************************************************* */
