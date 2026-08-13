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

#define CHANNEL             20          // RF Channel (11-26)
#define PAN_ID              0x2301      // Personal Area Network ID

/*----------------------------------------------------------------------------
 * ADC CONFIGURATION
 *----------------------------------------------------------------------------*/

#define ADC_CHANNEL         0           // AIN0 / P0.0

/*
 * CC2530 ADC:
 * 0x20 = Decimation by 256 = 12-bit resolution
 */
#define ADC_DEC_256         0x20

/*
 * 0x80 = AVDD5 as ADC reference voltage
 */
#define ADC_REF_AVDD        0x80

/*
 * Supply/reference voltage used for voltage calculation.
 * Change this value if the actual CC2530 AVDD voltage is different.
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
    /*
     * Configure BasicRF
     */
    rfConfig.myAddr = MY_ADDR;
    rfConfig.panId = PAN_ID;
    rfConfig.channel = CHANNEL;

    /*
     * Request acknowledgement from the receiver.
     */
    rfConfig.ackRequest = TRUE;

    /*
     * Initialize BasicRF.
     */
    basicRfInit(&rfConfig);
}

/*----------------------------------------------------------------------------
 * ADC INITIALIZATION
 *
 * P0.0 / AIN0 is configured as an analog input.
 *----------------------------------------------------------------------------*/

void ADC_Init(void)
{
    /*
     * Enable analog function on P0.0 / AIN0.
     */
    APCFG |= (1 << ADC_CHANNEL);

    /*
     * Enable ADC channel 0.
     */
    ADCCFG |= (1 << ADC_CHANNEL);

    /*
     * Configure P0.0 as input.
     */
    P0DIR &= ~(1 << ADC_CHANNEL);
}

/*----------------------------------------------------------------------------
 * ADC READ
 *
 * Reads AIN0 using:
 *
 * - AVDD5 reference
 * - 256 decimation
 * - 12-bit resolution
 *
 * The CC2530 ADC result is stored in ADCH:ADCL.
 * For 12-bit resolution, the result is right-shifted by 4 bits.
 *----------------------------------------------------------------------------*/

uint16_t ADC_Get_Value(void)
{
    uint16_t value;

    /*
     * Start a single ADC conversion.
     *
     * ADCCON3:
     * [7:6] = 10 -> AVDD5 reference
     * [5:4] = 10 -> 256 decimation / 12-bit resolution
     * [3:0] = 0000 -> AIN0
     */
    ADCCON3 = ADC_REF_AVDD |
              ADC_DEC_256 |
              ADC_CHANNEL;

    /*
     * Wait until conversion is complete.
     */
    while (!(ADCCON1 & 0x80))
    {
        /* Wait */
    }

    /*
     * Read the complete ADC result.
     */
    value = (uint16_t)ADCL;
    value |= ((uint16_t)ADCH << 8);

    /*
     * For 12-bit resolution, the four least-significant
     * bits are discarded.
     */
    value >>= 4;

    /*
     * Disable ADC channel after conversion.
     */
    ADCCFG &= ~(1 << ADC_CHANNEL);

    return value;
}

/*----------------------------------------------------------------------------
 * CONVERT ADC VALUE TO VOLTAGE
 *
 * 12-bit ADC:
 * 0     -> 0V
 * 4095  -> ADC reference voltage
 *
 * The reference voltage is AVDD5.
 *----------------------------------------------------------------------------*/

float ADC_To_Voltage(uint16_t adcValue)
{
    return ((float)adcValue * ADC_REFERENCE_VOLTAGE) / 4095.0f;
}

/*----------------------------------------------------------------------------
 * MAIN FUNCTION - TRANSMITTER MODE
 *----------------------------------------------------------------------------*/

void main(void)
{
    /*----------------------------------------------------------------------
     * Initialize hardware
     *----------------------------------------------------------------------*/

    halBoardInit();
    halMcuInit();

    /*----------------------------------------------------------------------
     * Initialize ADC
     *----------------------------------------------------------------------*/

    ADC_Init();

    /*----------------------------------------------------------------------
     * Initialize BasicRF
     *----------------------------------------------------------------------*/

    BasicRf_Init();

    /*----------------------------------------------------------------------
     * Main loop
     *----------------------------------------------------------------------*/

    while (1)
    {
        /*--------------------------------------------------------------
         * Read light sensor
         *--------------------------------------------------------------*/

        adcValue = ADC_Get_Value();

        /*--------------------------------------------------------------
         * Convert ADC reading to voltage
         *--------------------------------------------------------------*/

        voltage = ADC_To_Voltage(adcValue);

        /*--------------------------------------------------------------
         * Convert voltage to ASCII string
         *
         * Example:
         * "1.234"
         *--------------------------------------------------------------*/

        sprintf(
            (char *)txBuffer,
            "%.3f",
            voltage
        );

        /*--------------------------------------------------------------
         * Transmit voltage to Board A
         *--------------------------------------------------------------*/

        basicRfSendPacket(
            DEST_ADDR,
            txBuffer,
            strlen((char *)txBuffer)
        );

        /*--------------------------------------------------------------
         * Wait approximately 1 second before next reading
         *--------------------------------------------------------------*/

        halMcuWaitMs(1000);
    }
}
