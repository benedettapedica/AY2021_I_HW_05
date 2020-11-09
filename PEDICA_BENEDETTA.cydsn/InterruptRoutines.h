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
#ifndef _INTERRUPT_ROUTINE_H_
    #define _INTERRUPT_ROUTINE_H_

    #include "project.h"
    #include "cytypes.h"
    #include "stdio.h"
        
    #define BUTTON_PRESSED 0  //this MACRO is used for the flag in the ISR calling
      
    volatile uint8 flag_button;
        
    CY_ISR_PROTO(Custom_Button_ISR); //ISR triggered by the pressing of the button
        
#endif
/* [] END OF FILE */
