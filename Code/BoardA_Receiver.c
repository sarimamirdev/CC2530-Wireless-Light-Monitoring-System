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
 * DEFINES - Network Configuration
 *----------------------------------------------------------------------------*/

#define MY_ADDR         0x0001      // Board A (Receiver) address
#define DEST_ADDR       0x0002      // Board B (Transmitter) address
#define CHANNEL         20          // RF Channel (11-26)
#define PAN_ID          0x2301      // Personal Area Network ID
#define UART_BAUDRATE   115200

/*----------------------------------------------------------------------------
 * STUDENT INFORMATION
 * Modify these values as required.
 *----------------------------------------------------------------------------*/

#define STUDENT_NAME    "Zhiyong"
#define STUDENT_CLASS   "2401"

/*----------------------------------------------------------------------------
 * VOLTAGE THRESHOLDS
 *----------------------------------------------------------------------------*/

#define BRIGHT_THRESHOLD    1.5f
#define DARK_THRESHOLD      0.5f

/*----------------------------------------------------------------------------
 * GLOBAL VARIABLES
 *----------------------------------------------------------------------------*/

static basicRfCfg_t rfConfig;

/* RF receive buffer */
uint8_t rxBuffer[100];

/* Number of bytes received */
uint8_t rxLen;

/* Converted light sensor voltage */
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
    /* Configure BasicRF */
    rfConfig.myAddr = MY_ADDR;
    rfConfig.panId = PAN_ID;
    rfConfig.channel = CHANNEL;
    rfConfig.ackRequest = TRUE;

    /* Initialize BasicRF */
    basicRfInit(&rfConfig);

    /* Enable RF receive mode */
    basicRfReceiveOn();
}

/*----------------------------------------------------------------------------
 * CONTROL LEDs BASED ON VOLTAGE THRESHOLDS
 *
 * LEDs are active-low:
 * LOW  = LED ON
 * HIGH = LED OFF
 *
 * Voltage > 1.5V:
 *     Bright light  -> Both LEDs OFF
 *
 * 0.5V - 1.5V:
 *     Moderate light -> LED1 ON, LED2 OFF
 *
 * Voltage < 0.5V:
 *     Dark -> Both LEDs ON
 *----------------------------------------------------------------------------*/

void Control_LEDs(float voltage)
{
    /* Bright light - both LEDs OFF */
    if (voltage > BRIGHT_THRESHOLD)
    {
        P1_0 = 1;
        P1_1 = 1;
    }

    /* Dark - both LEDs ON */
    else if (voltage < DARK_THRESHOLD)
    {
        P1_0 = 0;
        P1_1 = 0;
    }

    /* Moderate light - LED1 ON, LED2 OFF */
    else
    {
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
 * MAIN FUNCTION - RECEIVER MODE
 *----------------------------------------------------------------------------*/

void main(void)
{
    /*----------------------------------------------------------------------
     * Initialize hardware
     *----------------------------------------------------------------------*/

    halBoardInit();
    halMcuInit();

    /* Initialize UART */
    halUartInit(0, UART_BAUDRATE);

    /* Initialize LEDs as outputs and turn them OFF */
    P1_0 = 1;
    P1_1 = 1;

    /* Initialize BasicRF */
    BasicRf_Init();

    /*----------------------------------------------------------------------
     * Main loop
     *----------------------------------------------------------------------*/

    while (1)
    {
        /* Check whether a BasicRF packet has been received */
        if (basicRfPacketIsReady())
        {
            /*
             * Receive packet.
             *
             * Leave one byte free for the null terminator because
             * atof() expects a null-terminated string.
             */
            rxLen = basicRfReceive(
                rxBuffer,
                sizeof(rxBuffer) - 1
            );

            /* Make sure valid data was received */
            if (rxLen > 0)
            {
                /* Null-terminate received string */
                rxBuffer[rxLen] = '\0';

                /* Convert received ASCII voltage to float */
                voltage = atof((char *)rxBuffer);

                /* Send measured voltage to PC */
                Send_To_PC(voltage);

                /* Update LEDs according to voltage */
                Control_LEDs(voltage);
            }

            /*
             * Make sure receiver remains enabled after
             * processing the packet.
             */
            basicRfReceiveOn();
        }

        /* Allow other tasks to run */
        halMcuWaitMs(10);
    }
}
