/* ========================================
 *
 * Brief .c file to define the ISR related to Debouncer of push button.
 * \author Benedetta Pedica
 * \date: 14 November 2020
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
