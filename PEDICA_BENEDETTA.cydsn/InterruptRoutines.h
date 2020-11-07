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
    
    #define LIS3DH_DEVICE_ADDRESS 0x18  //address of the slave device
    #define LIS3DH_CTRL_REG1  0x20
    
    #define CTRL_REG1_FREQ1   0x17
    #define CTRL_REG1_FREQ10  0x27
    #define CTRL_REG1_FREQ25  0x37
    #define CTRL_REG1_FREQ50  0x47
    #define CTRL_REG1_FREQ100 0x57
    #define CTRL_REG1_FREQ200 0x67
    
    CY_ISR_PROTO(Custom_Button_ISR); //ISR triggered by the pressing of the button
        
#endif
/* [] END OF FILE */
