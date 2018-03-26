
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
#include "testTcp.h"
#include "BCDS_CmdProcessor.h"
#include "BCDS_Assert.h"

#include "Serval_Tcp.h"
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
/* constant definitions ***************************************************** */

/* local variables ********************************************************** */
static CmdProcessor_T *AppCmdProcessor; /**< Application Command Processor Instance */
xTimerHandle wifiSendTimerHandle = NULL;
/* global variables ********************************************************* */

/* inline functions ********************************************************* */

/* local functions ********************************************************** */

/* global functions ********************************************************* */

Ip_Address_T *serverIP;

int portLocal = 9876;
Tcp_Socket_T* myTcpSocket=NULL;

void dataReceived(){
	printf("Data received!\r\n");
}

Callable_T tcpConnectCallback(){
	printf("TCP connect callback\r\n");
}

static void wifiTcpSend(void * param1, uint32_t port){
	BCDS_UNUSED(param1);
	BCDS_UNUSED(port);
	vTaskDelay(5000);
	retcode_t status;
	*serverIP= UINT32_C(0xC0A82B39);

	status = Tcp_listen(Ip_Port_T port, Callable_T *callback,
	                     Tcp_Listener_T *listener_ptr);
	retcode_t Tcp_accept(Tcp_Listener_T listener, Callable_T *callback_ptr,
	                     Tcp_Socket_T *socket_ptr);


	if(myTcpSocket==NULL){
		printf("Before TCP connect...\r\n");
		status = Tcp_connect(serverIP, portLocal, tcpConnectCallback, myTcpSocket);
		if(status!=RC_OK){
			printf("Error at TCP connecting: %d\r\n", status);
		}
		else{
			printf("TCP connection successful\r\n");
		}
	}

	int test = Tcp_isValidSocket(myTcpSocket);
	printf("Tcp socket is valid?:%d", test);
//
//	bool Tcp_isConnected(Tcp_Socket_T const socket);
//	retcode_t Tcp_getSocketStatus(
//	    Tcp_Socket_T socket, Tcp_SocketStatus_T *status_ptr);
//	retcode_t Tcp_getPeerName(Tcp_Socket_T socket, Ip_Address_T *ipAddr_ptr,
//	                          Ip_Port_T *port_ptr);
//	retcode_t Tcp_prepareForSending(
//	    Tcp_Socket_T const socket, MsgSendingCtx_T *sendingCtx_ptr);
//	retcode_t Tcp_send(Tcp_Socket_T socket, CommBuff_T packet,
//	                   Callable_T *callback_ptr);


	status = Tcp_close(myTcpSocket);
	status = Tcp_delete(myTcpSocket);

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

    Retcode_T retVal = CmdProcessor_Enqueue(AppCmdProcessor, wifiTcpSend, NULL, SERVER_PORT);
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

    memset(&myIpSettings, (uint32_t) 0, sizeof(myIpSettings));

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

static void init(void)
{
    Retcode_T retval = RETCODE_OK;
    static_assert((portTICK_RATE_MS != 0), "Tick rate MS is zero");

    /* create timer task to get and transmit accel data via BLE for every one second automatically*/
    wifiSendTimerHandle = xTimerCreate((char * const ) "wifiSend", 10000, TIMER_AUTORELOAD_ON, NULL, EnqueueDatatoWifi);

    if (NULL != wifiSendTimerHandle)
    {
        retval = wifiConnect();
    }
    if ((RETCODE_OK != retval) || (NULL == wifiSendTimerHandle))
    {
        printf("Failed to initialize because of Wifi/Command Processor/Timer Create fail \r\n");
        assert(false);
    }
}

void appInitSystem(void * CmdProcessorHandle, uint32_t param2)
{
    if (CmdProcessorHandle == NULL)
    {
        printf("Command processor handle is null \n\r");
        assert(false);
    }
    BCDS_UNUSED(param2);
    AppCmdProcessor = (CmdProcessor_T *) CmdProcessorHandle;
        /*Call the WNS module init API */
        init();
}
/**@} */
/** ************************************************************************* */
