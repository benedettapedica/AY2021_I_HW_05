/* ========================================
 *
 * File .h to define function for ISR 
 * \author Benedetta Pedica
 * \date: 14 November 2020
 *
 * ========================================
*/
#ifndef _INTERRUPT_ROUTINE_H_
    #define _INTERRUPT_ROUTINE_H_

    #include "project.h"
    #include "cytypes.h"
    #include "stdio.h"
        
    #define BUTTON_PRESSED 0  //this MACRO is used for the flag in the ISR calling
      
    volatile uint8 flag_button; //variable that keeps track of the button: is 0 when button is pressed
        
    CY_ISR_PROTO(Custom_Button_ISR); //ISR triggered by the pressing of the button
        
#endif
/* [] END OF FILE */
