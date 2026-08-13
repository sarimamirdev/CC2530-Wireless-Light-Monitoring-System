/*----------------------------------------------------------------------------
 * CC2530 - Board B (Transmitter)
 * 
 * Description: Reads light sensor, transmits data via BasicRF
 *----------------------------------------------------------------------------*/

#include "hal_defs.h"
#include "hal_board.h"
#include "hal_mcu.h"
#include "hal_adc.h"
#include "basic_rf.h"
#include <stdio.h>
#include <string.h>

/*----------------------------------------------------------------------------
 * DEFINES - Network Configuration
 *----------------------------------------------------------------------------*/
#define MY_ADDR         0x0002      // Board B address
#define DEST_ADDR       0x0001      // Destination address (Board A)
#define CHANNEL         20          // RF Channel (11-26)
#define PAN_ID          0x2301      // Personal Area Network ID
#define ADC_CHANNEL     0           // P0_0 as ADC input

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
 * ADC INITIALIZATION
 *----------------------------------------------------------------------------*/
void ADC_Init(void)
{
    // Configure P0_0 as analog input
    P0SEL |= 0x01;          // P0.0 as peripheral (analog)
    P0DIR &= ~0x01;         // P0.0 as input
    APCFG |= 0x01;          // Enable ADC on P0.0
}

/*----------------------------------------------------------------------------
 * ADC READ (12-bit, 0-3.3V)
 *----------------------------------------------------------------------------*/
uint16_t ADC_Get_Value(void)
{
    uint16_t value = 0;
    
    // Configure ADC: 12-bit, P0_0
    ADCCON3 = (0x30 | (ADC_CHANNEL << 4) | 0x10);
    
    // Start conversion
    ADCCON1 |= 0x30;
    
    // Wait for conversion complete
    while (!(ADCCON1 & 0x80));
    
    // Read result
    value = (ADCH << 8) | (ADCL & 0x03);
    value >>= 1;            // 12-bit right-aligned
    
    return value;
}

/*----------------------------------------------------------------------------
 * CONVERT ADC VALUE TO VOLTAGE
 *----------------------------------------------------------------------------*/
float ADC_To_Voltage(uint16_t adcValue)
{
    return (adcValue * 3.3f) / 4095.0f;
}

/*----------------------------------------------------------------------------
 * MAIN FUNCTION - Transmitter Mode
 *----------------------------------------------------------------------------*/
void main(void)
{
    // Initialize hardware
    halBoardInit();
    halMcuInit();
    
    // Initialize ADC
    ADC_Init();
    
    // Initialize RF
    BasicRf_Init();
    
    while (1) {
        // Read sensor
        adcValue = ADC_Get_Value();
        voltage = ADC_To_Voltage(adcValue);
        
        // Format and send via BasicRF to Board A
        sprintf((char*)txBuffer, "%.3f", voltage);
        basicRfSendPacket(DEST_ADDR, txBuffer, strlen((char*)txBuffer));
        
        // Wait 1 second
        halMcuWaitMs(1000);
    }
}
