/* ========================================
 *
 * Header file where are declared the functions used to read, write and update the registers
 * and the I2C peripheral is initialized.
 * \author Benedetta Pedica
 * \date: 14 November 2020
 *
 * ========================================
*/
#ifndef __I2C_INTERFACE_H__
    #define __I2C_INTERFACE_H__
    
    #include "cytypes.h"
    #include "ErrorCodes.h" 
    #include "project.h"
    
    ErrorCode I2C_Peripheral_Start(void); //Start I2C peripheral
    ErrorCode I2C_Peripheral_Stop(void);  //Stop I2C peripheral
    
    uint8 I2C_IsDeviceConnected(uint8_t device_address);
    
    //Read a register and save it in a variable
    ErrorCode I2C_Peripheral_ReadRegister(uint8_t device_address, 
                                            uint8_t register_address,
                                            uint8_t* data);
    // Write the content of a variable in a register
    ErrorCode I2C_Peripheral_WriteRegister(uint8_t device_address,
                                            uint8_t register_address,
                                            uint8_t data);

    
#endif


/* [] END OF FILE */
