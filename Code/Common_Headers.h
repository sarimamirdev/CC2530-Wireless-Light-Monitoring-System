/*----------------------------------------------------------------------------
 * CC2530 - Common Headers
 * 
 * Description: Shared definitions and includes for both boards
 *----------------------------------------------------------------------------*/

#ifndef COMMON_HEADERS_H
#define COMMON_HEADERS_H

#include "hal_defs.h"
#include "hal_board.h"
#include "hal_mcu.h"
#include "hal_uart.h"
#include "hal_adc.h"
#include "basic_rf.h"
#include <stdio.h>
#include <string.h>

/*----------------------------------------------------------------------------
 * NETWORK CONFIGURATION
 *----------------------------------------------------------------------------*/
#define CHANNEL         20          // RF Channel (11-26)
#define PAN_ID          0x2301      // Personal Area Network ID
#define UART_BAUDRATE   115200

/*----------------------------------------------------------------------------
 * STUDENT INFORMATION (Modify as needed)
 *----------------------------------------------------------------------------*/
#define STUDENT_NAME    "Zhiyong"
#define STUDENT_CLASS   "2401"

#endif /* COMMON_HEADERS_H */
