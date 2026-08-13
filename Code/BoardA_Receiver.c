/*----------------------------------------------------------------------------
 * CC2530 - Board A (Receiver)
 * 
 * Description: Receives data via BasicRF, controls LEDs, sends to PC via UART
 *----------------------------------------------------------------------------*/

#include "hal_defs.h"
#include "hal_board.h"
#include "hal_mcu.h"
#include "hal_uart.h"
#include "basic_rf.h"
#include <stdio.h>
#include <string.h>

/*----------------------------------------------------------------------------
 * DEFINES - Network Configuration
 *----------------------------------------------------------------------------*/
#define MY_ADDR         0x0001      // Board A address
#define DEST_ADDR       0x0002      // Destination address (Board B)
#define CHANNEL         20          // RF Channel (11-26)
#define PAN_ID          0x2301      // Personal Area Network ID
#define UART_BAUDRATE   115200

// Student info (modify as needed)
#define STUDENT_NAME    "Zhiyong"
#define STUDENT_CLASS   "2401"

/*----------------------------------------------------------------------------
 * GLOBAL VARIABLES
 *----------------------------------------------------------------------------*/
static basicRfCfg_t rfConfig;
uint8_t rxBuffer[100];
uint8_t rxLen;
float voltage;

/*----------------------------------------------------------------------------
 * FUNCTION PROTOTYPES
 *----------------------------------------------------------------------------*/
void BasicRf_Init(void);
void Control_LEDs(float voltage);
void Send_To_PC(float voltage);

/*----------------------------------------------------------------------------
 * RF INITIALIZATION
 *----------------------------------------------------------------------------*/
void BasicRf_Init(void)
{
    rfConfig.myAddr = MY_ADDR;
    rfConfig.panId = PAN_ID;
    rfConfig.channel = CHANNEL;
    rfConfig.ackRequest = TRUE;
    
    basicRfInit(&rfConfig);
}

/*----------------------------------------------------------------------------
 * CONTROL LEDs BASED ON VOLTAGE THRESHOLDS
 *----------------------------------------------------------------------------*/
void Control_LEDs(float voltage)
{
    // LED ON = Pin LOW (Active Low)
    if (voltage > 1.5f) {
        // All LEDs OFF (Bright light)
        P1_0 = 1;
        P1_1 = 1;
    } else if (voltage < 0.5f) {
        // Two LEDs ON (Dark)
        P1_0 = 0;
        P1_1 = 0;
    } else {
        // One LED ON (Moderate light)
        P1_0 = 0;
        P1_1 = 1;
    }
}

/*----------------------------------------------------------------------------
 * SEND DATA TO PC VIA UART
 *----------------------------------------------------------------------------*/
void Send_To_PC(float voltage)
{
    char output[150];
    
    sprintf(output, "The illumination data collected by Student %s of IoT Class %s is: %.3fV\r\n", 
            STUDENT_NAME, STUDENT_CLASS, voltage);
    
    halUartWrite((uint8_t*)output, strlen(output));
}

/*----------------------------------------------------------------------------
 * MAIN FUNCTION - Receiver Mode
 *----------------------------------------------------------------------------*/
void main(void)
{
    // Initialize hardware
    halBoardInit();
    halMcuInit();
    halUartInit(0, UART_BAUDRATE);
    
    // Initialize RF
    BasicRf_Init();
    
    // Initialize LEDs (OFF)
    P1_0 = 1;
    P1_1 = 1;
    
    while (1) {
        // Check for incoming packet
        if (basicRfPacketIsReady()) {
            rxLen = basicRfReceive(rxBuffer, sizeof(rxBuffer));
            
            if (rxLen > 0) {
                // Convert received data to voltage
                voltage = atof((char*)rxBuffer);
                
                // Send to PC
                Send_To_PC(voltage);
                
                // Control LEDs
                Control_LEDs(voltage);
            }
        }
        
        // Allow other tasks
        halMcuWaitMs(10);
    }
}
