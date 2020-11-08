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
#include "I2C_Interface.h"


CY_ISR(Custom_Button_ISR)
{      
   flag_button= BUTTON_PRESSED;   
}

/* [] END OF FILE */
