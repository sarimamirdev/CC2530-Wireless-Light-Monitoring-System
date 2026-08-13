/*----------------------------------------------------------------------------
 * CC2530 - Board B (Transmitter)
 *
 * Description:
 * Reads light intensity using the CC2530 ADC and transmits
 * the measured voltage to Board A via BasicRF.
 *----------------------------------------------------------------------------*/

#include "hal_defs.h"
#include "hal_board.h"
#include "hal_mcu.h"
#include "hal_adc.h"
#include "basic_rf.h"

#include <stdio.h>
#include <string.h>

/*----------------------------------------------------------------------------
 * NETWORK CONFIGURATION
 *----------------------------------------------------------------------------*/

#define MY_ADDR             0x0002      // Board B (Transmitter)
#define DEST_ADDR           0x0001      // Board A (Receiver)

#define CHANNEL             20
#define PAN_ID              0x2301

/*----------------------------------------------------------------------------
 * ADC CONFIGURATION
 *----------------------------------------------------------------------------*/

#define ADC_CHANNEL         0           // P0.0 / AIN0

/*
 * CC2530 HAL:
 * 256 decimation is used for 12-bit ADC resolution.
 */
#define ADC_DEC_256         0x20

/*
 * AVDD5 is used as the ADC reference.
 */
#define ADC_REF_AVDD        0x80

/*
 * Assumed AVDD/reference voltage.
 */
#define ADC_REFERENCE_VOLTAGE   3.3f

/*----------------------------------------------------------------------------
 * GLOBAL VARIABLES
 *----------------------------------------------------------------------------*/

static basicRfCfg_t rfConfig;

uint8_t txBuffer[100];

uint16_t adcValue;

float voltage;

/*----------------------------------------------------------------------------
 * FUNCTION PROTOTYPES
 *----------------------------------------------------------------------------*/

void BasicRf_Init(void);
void ADC_Init(void);
uint16_t ADC_Get_Value(void);
float ADC_To_Voltage(uint16_t adcValue);

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
}

/*----------------------------------------------------------------------------
 * ADC INITIALIZATION
 *
 * P0.0 / AIN0 is configured as an analog input.
 *----------------------------------------------------------------------------*/

void ADC_Init(void)
{
    /* Enable analog input on P0.0 */
    APCFG |= (1 << ADC_CHANNEL);

    /* Configure P0.0 as input */
    P0DIR &= ~(1 << ADC_CHANNEL);
}

/*----------------------------------------------------------------------------
 * ADC READ
 *
 * Reads AIN0 using AVDD5 as reference and
 * 256-decimation / 12-bit resolution.
 *----------------------------------------------------------------------------*/

uint16_t ADC_Get_Value(void)
{
    uint16_t value;

    /*
     * Configure ADC conversion:
     *
     * AVDD5 reference
     * 256 decimation / 12-bit resolution
     * AIN0 input channel
     */
    ADCCON3 = ADC_REF_AVDD |
              ADC_DEC_256 |
              ADC_CHANNEL;

    /* Wait for conversion to complete */
    while (!(ADCCON1 & 0x80))
    {
        /* Wait */
    }

    /* Read ADC result */
    value = (uint16_t)ADCL;
    value |= ((uint16_t)ADCH << 8);

    /*
     * Convert ADC result to 12-bit value.
     */
    value >>= 4;

    return value;
}

/*----------------------------------------------------------------------------
 * ADC VALUE TO VOLTAGE
 *
 * 12-bit ADC range:
 *
 * 0     -> 0V
 * 4095  -> reference voltage
 *
 * The conversion assumes AVDD = 3.3V.
 *----------------------------------------------------------------------------*/

float ADC_To_Voltage(uint16_t adcValue)
{
    return ((float)adcValue * ADC_REFERENCE_VOLTAGE) / 4095.0f;
}

/*----------------------------------------------------------------------------
 * MAIN FUNCTION
 *----------------------------------------------------------------------------*/

void main(void)
{
    /* Initialize hardware */
    halBoardInit();
    halMcuInit();

    /* Initialize ADC */
    ADC_Init();

    /* Initialize BasicRF */
    BasicRf_Init();

    while (1)
    {
        /* Read light sensor */
        adcValue = ADC_Get_Value();

        /* Convert ADC reading to voltage */
        voltage = ADC_To_Voltage(adcValue);

        /* Convert voltage to ASCII */
        sprintf(
            (char *)txBuffer,
            "%.3f",
            voltage
        );

        /*
         * Send voltage to Board A.
         */
        basicRfSendPacket(
            DEST_ADDR,
            txBuffer,
            strlen((char *)txBuffer)
        );

        /* Send approximately once per second */
        halMcuWaitMs(1000);
    }
}
