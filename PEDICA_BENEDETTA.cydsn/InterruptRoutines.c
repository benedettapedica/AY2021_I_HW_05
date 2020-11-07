/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "InterruptRoutines.h"

CY_ISR(Button_ISR)
{   
    EEPROM_UpdateTemperature(); //security measure if temperature has changed of few degrees
}

/* [] END OF FILE */
