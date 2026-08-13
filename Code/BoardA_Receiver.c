/*----------------------------------------------------------------------------
 * CC2530 - Board A (Receiver)
 *
 * Description:
 * Receives light-intensity data via BasicRF,
 * controls LEDs according to voltage level,
 * and sends formatted data to PC via UART.
 *----------------------------------------------------------------------------*/

#include "hal_defs.h"
#include "hal_board.h"
#include "hal_mcu.h"
#include "hal_uart.h"
#include "basic_rf.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*----------------------------------------------------------------------------
 * NETWORK CONFIGURATION
 *----------------------------------------------------------------------------*/

#define MY_ADDR         0x0001      // Board A (Receiver)
#define DEST_ADDR       0x0002      // Board B (Transmitter)
#define CHANNEL         20          // RF Channel
#define PAN_ID          0x2301      // Personal Area Network ID
#define UART_BAUDRATE   115200

/*----------------------------------------------------------------------------
 * STUDENT INFORMATION
 *----------------------------------------------------------------------------*/

#define STUDENT_NAME    "Zhiyong"
#define STUDENT_CLASS   "2401"

/*----------------------------------------------------------------------------
 * LIGHT LEVEL THRESHOLDS
 *----------------------------------------------------------------------------*/

#define BRIGHT_THRESHOLD    1.5f
#define DARK_THRESHOLD      0.5f

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
 * BASICRF INITIALIZATION
 *----------------------------------------------------------------------------*/

void BasicRf_Init(void)
{
    rfConfig.myAddr = MY_ADDR;
    rfConfig.panId = PAN_ID;
    rfConfig.channel = CHANNEL;
    rfConfig.ackRequest = TRUE;

    basicRfInit(&rfConfig);

    /* Enable RF receive mode */
    basicRfReceiveOn();
}

/*----------------------------------------------------------------------------
 * LED CONTROL
 *
 * Active-low LEDs:
 * LOW  = ON
 * HIGH = OFF
 *
 * > 1.5V       -> Bright   -> Both LEDs OFF
 * 0.5V - 1.5V -> Moderate -> LED1 ON
 * < 0.5V       -> Dark     -> Both LEDs ON
 *----------------------------------------------------------------------------*/

void Control_LEDs(float voltage)
{
    if (voltage > BRIGHT_THRESHOLD)
    {
        /* Bright light */
        P1_0 = 1;
        P1_1 = 1;
    }
    else if (voltage < DARK_THRESHOLD)
    {
        /* Dark */
        P1_0 = 0;
        P1_1 = 0;
    }
    else
    {
        /* Moderate light */
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

    sprintf(
        output,
        "The illumination data collected by Student %s of IoT Class %s is: %.3fV\r\n",
        STUDENT_NAME,
        STUDENT_CLASS,
        voltage
    );

    halUartWrite(
        (uint8_t *)output,
        strlen(output)
    );
}

/*----------------------------------------------------------------------------
 * MAIN FUNCTION
 *----------------------------------------------------------------------------*/

void main(void)
{
    /* Initialize hardware */
    halBoardInit();
    halMcuInit();

    /* Initialize UART */
    halUartInit(0, UART_BAUDRATE);

    /* Initialize LEDs as OFF */
    P1_0 = 1;
    P1_1 = 1;

    /* Initialize BasicRF */
    BasicRf_Init();

    while (1)
    {
        /* Check for incoming RF packet */
        if (basicRfPacketIsReady())
        {
            /*
             * Leave one byte free for '\0'
             * so the received data can safely
             * be processed as a C string.
             */
            rxLen = basicRfReceive(
                rxBuffer,
                sizeof(rxBuffer) - 1
            );

            if (rxLen > 0)
            {
                /* Null-terminate received data */
                rxBuffer[rxLen] = '\0';

                /* Convert received ASCII voltage to float */
                voltage = atof((char *)rxBuffer);

                /* Send data to PC */
                Send_To_PC(voltage);

                /* Update LEDs */
                Control_LEDs(voltage);
            }

            /* Ensure receiver remains active */
            basicRfReceiveOn();
        }

        /* Small delay */
        halMcuWaitMs(10);
    }
}
