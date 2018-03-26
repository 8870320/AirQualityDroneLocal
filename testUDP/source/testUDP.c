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
* @defgroup SEND_DATA_OVER_UDP Send Data Over UDP
* @{
*
* @brief Send Data Over Udp
* @details This module is intended to send UDP data to a server side. Here the device connects to an access point and obtains an IP address via DHCP. Thereafter it triggers a task for reading data every pre-defined time interval,
* and sending it over WIFI as a UDP broadcast to a pre-defined server listening on a pre-defined port.
*
* @file
**/

/* module includes ********************************************************** */
#include "XDKAppInfo.h"
#undef BCDS_MODULE_ID  /* Module ID define before including Basics package*/
#define BCDS_MODULE_ID XDK_APP_MODULE_ID_DATA_OVER_UDP

/* own header files */
#include "testUDP.h"
#include "XdkSensorHandle.h"

/* system header files */
#include <stdio.h>

/* additional interface header files */
#include "simplelink.h"
#include "BCDS_Basics.h"
#include "BCDS_Assert.h"
#include "BCDS_CmdProcessor.h"
#include "BCDS_WlanConnect.h"
#include "BCDS_NetworkConfig.h"
#include "Serval_Types.h"
#include "Serval_Ip.h"
#include "FreeRTOS.h"
#include "timers.h"

#define RECV_BUF_LEN 20
#define SEND_BUF_LEN 20
/* constant definitions ***************************************************** */

/* local variables ********************************************************** */

static CmdProcessor_T *AppCmdProcessor; /**< Application Command Processor Instance */

/**
 * This buffer holds the data to be sent to server via UDP
 * */
static uint16_t bsdBuffer_mau[BUFFER_SIZE] = { (uint16_t) ZERO };
/**
 * Timer handle for periodically sending data over wifi
 */
xTimerHandle wifiSendTimerHandle = NULL;

_i16 socketHandle=NULL;
Ip_Address_T destAddr = SERVER_IP;
SlSockAddrIn_t addr;




/**
 * @brief Opening a TCP client side socket and sending data on a server port
 *
 * This function opens a UDP socket and tries to connect to a Server SERVER_IP
 * waiting on port SERVER_PORT.
 * Then the function will send periodic UDP packets to the server.
 * 
 * @param[in]   *param1     generic pointer to any context data structure which will be passed to the function.
 *
 * @param[in]   port        destination port number
 *
 *
 */
static void wifiTcpSend(void * param1, uint32_t port){
//	// Receiving data from a TCP socket
//
//	SlSockAddrIn_t  Addr;
//	SlSockAddrIn_t  LocalAddr;
//	_i16 AddrSize = sizeof(SlSockAddrIn_t);
//	_i16 SockID, newSockID;
//	_i16 Status;
//	_i8 Buf[RECV_BUF_LEN];
//
//	LocalAddr.sin_family = SL_AF_INET;
//	LocalAddr.sin_port = sl_Htons(6666);
//	LocalAddr.sin_addr.s_addr = sl_Htonl(0xC0A82B39);
//
//	Addr.sin_family = SL_AF_INET;
//	Addr.sin_port = sl_Htons(6666);
//	Addr.sin_addr.s_addr = sl_Htonl(SL_IPV4_VAL(192,168,43,57));
//
//	printf("LocalAdr: %d, Adr: %d", LocalAddr.sin_addr.s_addr,Addr.sin_addr.s_addr);
//
//	SockID = sl_Socket(SL_AF_INET,SL_SOCK_STREAM, 0);
//	Status = sl_Bind(SockID, (SlSockAddr_t *)&LocalAddr, AddrSize);
//	Status = sl_Listen(SockID, 0);
//	newSockID = sl_Accept(SockID, (SlSockAddr_t*)&Addr, (SlSocklen_t*) &AddrSize);
//	Status = sl_Recv(newSockID, Buf, 1460, 0);
//
//	printf(Buf[0]);


	int status;

//	BCDS_UNUSED(param1);
//	SlSockAddrIn_t  Addr;
//	_i16 AddrSize = sizeof(SlSockAddrIn_t);
//	_i16 SockID;
//	_i16 Status;
//	_i8 Buf[RECV_BUF_LEN];
//
//	bsdBuffer_mau[0] = 1;
//	Buf[1] = 1;//0xAA55;
//	Buf[2] = 2;//0xBB66;
//	Buf[3] = 3;//0xCC77;
//
//	Addr.sin_family = SL_AF_INET;
//	Addr.sin_port = sl_Htons((uint16_t) port);
//	Addr.sin_addr.s_addr = sl_Htonl(SERVER_IP);
//	AddrSize = sizeof(SlSockAddrIn_t);
//
//	SockID = sl_Socket(SL_AF_INET,SL_SOCK_STREAM, 0);
//	printf("Socket ID = %d  \r\n", SockID);
//	Status = sl_Connect(SockID, (SlSockAddr_t *)&Addr, AddrSize);
//	if (Status <= (int16_t) ZERO)
//	{
//		printf("Error in establishing connection, Code: %d\r\n", Status);
//	}
//	Status = sl_Send(SockID, Buf, sizeof(Buf), 0 );
//	if (Status <= (int16_t) ZERO)
//	{
//		printf("Error in sending data Code: %d\r\n",Status);
//	}
	vTaskDelay(5000);
	printf("send func started\r\n");


	char outBuf[1024];
	sprintf(outBuf, "Test TCP send\r\n");
	status=sl_Send(socketHandle, (const void *) outBuf, strlen(outBuf), 0);
	if(status != SL_RET_CODE_OK)
		{
			printf("Error in sending: %d\r\n",status);
			sl_Close(socketHandle);
		}
	else{
			printf("sent!");
		}
	printf("send func finished!\r\n");

}


/**
 * @brief Opening a UDP client side socket and sending data on a server port
 *
 * This function opens a UDP socket and tries to connect to a Server SERVER_IP
 * waiting on port SERVER_PORT.
 * Then the function will send periodic UDP packets to the server.
 *
 * @param[in]   *param1     generic pointer to any context data structure which will be passed to the function.
 *
 * @param[in]   port        destination port number
 *
 *
 */
static void wifiUdpSend(void * param1, uint32_t port)
{
    BCDS_UNUSED(param1);
    static uint16_t counter = UINT16_C(0);
    SlSockAddrIn_t Addr;
    uint16_t AddrSize = (uint16_t) ZERO;
    int16_t SockID = (int16_t) 6000;
    int16_t Status = (int16_t) ZERO;

    /* copies the dummy data to send array , the first array element is the running counter to track the number of packets send so far*/
    bsdBuffer_mau[0] = counter;
    bsdBuffer_mau[1] = 0xAA55;
    bsdBuffer_mau[2] = 0xBB66;
    bsdBuffer_mau[3] = 0xCC77;
    Addr.sin_family = SL_AF_INET;
    Addr.sin_port = sl_Htons((uint16_t) SERVER_PORT);
    Addr.sin_addr.s_addr = sl_Htonl(SERVER_IP);
    AddrSize = sizeof(SlSockAddrIn_t);

    SockID = sl_Socket(SL_AF_INET, SL_SOCK_DGRAM, (uint32_t) ZERO); /**<The return value is a positive number if successful; other wise negative*/
    if (SockID < (int16_t) ZERO)
    {
        printf("Error in socket opening\n");
        /* error case*/
        return;
    }
    char outBuf[1024];
	sprintf(outBuf, "Test TCP send\r\n");

	Status = sl_SendTo(SockID, (const void *) outBuf, strlen(outBuf), (uint32_t) ZERO, (SlSockAddr_t *) &Addr, AddrSize);/**<The return value is a number of characters sent;negative if not successful*/

//	Status = sl_SendTo(SockID, bsdBuffer_mau, BUFFER_SIZE * sizeof(uint16_t), (uint32_t) ZERO, (SlSockAddr_t *) &Addr, AddrSize);/**<The return value is a number of characters sent;negative if not successful*/
	/*Check if 0 transmitted bytes sent or error condition*/
    if (Status <= (int16_t) ZERO)
    {
        Status = sl_Close(SockID);
        if (Status < 0)
        {
            printf("Error is closing socket after failing to send the Udp data \r\n");
            return;
        }
        printf("Error in sending data \r\n");
        return;
    }
    Status = sl_Close(SockID);
    if (Status < 0)
    {
        printf("Error in closing socket after sending successfully sending the udp data \r\n");
        return;
    }
    counter++;
    printf("UDP sending successful\r\n");
    return;
}

/**
 * @brief        This is a application timer callback function used to enqueue SendAccelDataoverWifi function
 *               to the command processor.
 *
 * @param[in]    pvParameters unused parameter.
 */
static void EnqueueDatatoWifi(void *pvParameters)
{
    BCDS_UNUSED(pvParameters);

    printf("Before TCP Init \r\n");
    vTaskDelay(5000);

	tcpInit();
	printf("Before Task enqueueing\r\n");


    Retcode_T retVal = CmdProcessor_Enqueue(AppCmdProcessor, wifiUdpSend, NULL, SERVER_PORT);
    if (RETCODE_OK != retVal)
    {
    	if(RETCODE_FAILURE ==retVal)
    		printf("Queue is full \r\n");
        printf("Failed to Enqueue SendAccelDataoverWifi to Application Command Processor \r\n");
    }
}

/**
 *  @brief       Function to print the XDK Device IP Address on the USB.
 *
 */
static void PrintDeviceIP(void)
{
    NetworkConfig_IpSettings_T myIpSettings;
    Ip_Address_T* IpaddressHex = Ip_getMyIpAddr();
    int32_t Result = INT32_C(-1);
    char ipAddress[SERVAL_IP_ADDR_LEN] = { 0 };

    memset(&myIpSettings, (uint32_t) ZERO, sizeof(myIpSettings));

    Retcode_T ReturnValue = NetworkConfig_GetIpSettings(&myIpSettings);
    if (ReturnValue == RETCODE_OK)
    {
        *IpaddressHex = Basics_htonl(myIpSettings.ipV4);
        Result = Ip_convertAddrToString(IpaddressHex, ipAddress);
        if (Result < INT32_C(0))
        {
            printf("Couldn't convert the IP address to string format \r\n ");
        }
        printf(" Ip address of the device %s \r\n ", ipAddress);
    }
    else
    {
        printf("Error in getting IP settings\n\r");
    }
}

/**
 * @brief Callback function called on WIFI event
 *
 * @param [in]  Event : event to be send by WIFI during connection/disconnection.
 *
 *
 */
static void WlanEventCallback(WlanConnect_Status_T Event)
{
    switch (Event)
    {
    case WLAN_CONNECTED:
        /* start accelerometer data transmission timer */
        if (pdTRUE != xTimerStart(wifiSendTimerHandle, (WIFI_TX_FREQ/portTICK_RATE_MS)))
        {
            /* Assertion Reason : Failed to start software timer. Check command queue size of software timer service*/
            assert(false);
        }
        printf("XDK Device Connected over WIFI \r\n");
        PrintDeviceIP();
        break;
    case WLAN_DISCONNECTED:
        /* stop Ble timer accelerometer data transmission timer */
        if (pdTRUE != xTimerStop(wifiSendTimerHandle, UINT8_C(0)))
        {
            /* Assertion Reason: Failed to start software timer. Check command queue size of software timer service. */
            assert(false);
        }
        printf("XDK Device disconnected form WIFI n/w \r\n");
        break;
    case WLAN_CONNECTION_ERROR:
        printf("XDK Device WIFI Connection error \r\n");
        break;
    case WLAN_CONNECTION_PWD_ERROR:
        printf("XDK Device WIFI connection error due to wrong password \r\n");
        break;
    case WLAN_DISCONNECT_ERROR:
        printf("XDK Device WIFI Disconnect error \r\n");
        break;
    default:
        printf("XDK Device unknown WIFI event \r\n");
        break;
    }
}

/**
 * @brief        This function used to connect to wifi n/w with provided SSID & Password
 *
 * @retval       RETCODE_OK is API success or an error.
 */
static Retcode_T wifiConnect(void)
{
    WlanConnect_SSID_T connectSSID;
    WlanConnect_PassPhrase_T connectPassPhrase;
    Retcode_T ReturnValue = (Retcode_T) RETCODE_FAILURE;

    ReturnValue = WlanConnect_Init();

    if (RETCODE_OK != ReturnValue)
    {
        printf("Error occurred initializing WLAN \r\n ");
        return ReturnValue;
    }
    printf("Connecting to %s \r\n ", WLAN_CONNECT_WPA_SSID);

    connectSSID = (WlanConnect_SSID_T) WLAN_CONNECT_WPA_SSID;
    connectPassPhrase = (WlanConnect_PassPhrase_T) WLAN_CONNECT_WPA_PASS;
    ReturnValue = NetworkConfig_SetIpDhcp(NULL);
    if (RETCODE_OK != ReturnValue)
    {
        printf("Error in setting IP to DHCP\n\r");
        return ReturnValue;
    }
    ReturnValue = WlanConnect_WPA(connectSSID, connectPassPhrase, WlanEventCallback);
    if (RETCODE_OK != ReturnValue)
    {
        printf("Error occurred while connecting Wlan %s \r\n ", WLAN_CONNECT_WPA_SSID);
    }
    return ReturnValue;
}


void tcpInit(){
	printf("Start TCP init...\r\n");
	vTaskDelay(5000);
	printf("Start TCP init...finished\r\n");
	socketHandle = sl_Socket(SL_AF_INET, SL_SOCK_STREAM, IPPROTO_TCP);
	if(socketHandle<0){
		printf("Error in creating socket handle: %d\r\n",socketHandle);
	}
	else{
		printf("Socket handle initialized correctly\r\n");
	}


	addr.sin_family = SL_AF_INET;
	addr.sin_port = sl_Htons(SERVER_PORT);
	addr.sin_addr.s_addr = destAddr;
	printf("before connection...\r\n");
	int status =sl_Connect(socketHandle, ( SlSockAddr_t *)&addr, sizeof(SlSockAddrIn_t));
	if(status != SL_RET_CODE_OK)
	{
		printf("Some connection error: %d\r\n", status);
		sl_Close(socketHandle);
	}
	else{
		printf("connected!");
	}
}

/**
 *  @brief
 *      Function to initialize the wifi network send application. Create timer task
 *      to start WiFi Connect and get IP function after one second. After that another timer
 *      to send data periodically.
 */
static void init(void)
{
    Retcode_T retval = RETCODE_OK;
    static_assert((portTICK_RATE_MS != 0), "Tick rate MS is zero");



    /* create timer task to get and transmit accel data via BLE for every one second automatically*/

    wifiSendTimerHandle = xTimerCreate((char * const ) "wifiTcpSend", 10000, TIMER_AUTORELOAD_ON, NULL, EnqueueDatatoWifi);
    if (NULL != wifiSendTimerHandle)
    {
        retval = wifiConnect();

        if(retval==RETCODE_OK){
        	printf("wifi connect successful\r\n");
        }
        else{
        	printf("wifi connect not successful\r\n");
        }
    }
//    wifiTcpSend(null, 6666);
    if ((RETCODE_OK != retval|| (NULL == wifiSendTimerHandle))) //
    {
        printf("Failed to initialize because of Wifi/Command Processor/Timer Create fail \r\n");
        assert(false);
    }


}



/* global functions ********************************************************* */
void appInitSystem(void * CmdProcessorHandle, uint32_t param2)
{
    if (CmdProcessorHandle == NULL)
    {
        printf("Command processor handle is null \n\r");
        assert(false);
    }
    AppCmdProcessor = (CmdProcessor_T *) CmdProcessorHandle;
    BCDS_UNUSED(param2);
    /*Call the WNS module init API */
    init();
}
/**@} */
/** ************************************************************************* */
