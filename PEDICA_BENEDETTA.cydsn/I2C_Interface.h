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
#ifndef __I2C_INTERFACE_H__
    #define __I2C_INTERFACE_H__
    
    #include "cytypes.h"
    #include "ErrorCodes.h" 
    #include "project.h"
    
    ErrorCode I2C_Peripheral_Start(void);
    ErrorCode I2C_Peripheral_Stop(void);
    
    ErrorCode I2C_Peripheral_ReadRegister(uint8_t device_address, 
                                            uint8_t register_address,
                                            uint8_t* data);
    
    ErrorCode I2C_Peripheral_WriteRegister(uint8_t device_address,
                                            uint8_t register_address,
                                            uint8_t data);

#endif


/* [] END OF FILE */
