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
#include "I2C_Interface.h" 
#include "I2C.h"

    ErrorCode I2C_Peripheral_Start(void) 
    {
        // Start I2C peripheral
        I2C_Start();  
        // Return no error since start function does not return any error
        return NO_ERROR;
    }
    
    
    ErrorCode I2C_Peripheral_Stop(void)
    {
        // Stop I2C peripheral
        I2C_Stop();
        // Return no error since stop function does not return any error
        return NO_ERROR;
    }
    
    uint8 I2C_IsDeviceConnected(uint8 device_address)
     {
        //send a start condition followed by a stop condition
        uint8 connection = I2C_MasterSendStart(device_address, I2C_WRITE_XFER_MODE);
        I2C_MasterSendStop();

        if (connection == I2C_MSTR_NO_ERROR) //return 1 if the device is connected, 0 otherwise
            return 1; 
        else
            return 0;
     }

    ErrorCode I2C_Peripheral_ReadRegister(uint8_t device_address, 
                                            uint8_t register_address,
                                            uint8_t* data)
    {
        // Send start condition
        uint8_t error = I2C_MasterSendStart(device_address,I2C_WRITE_XFER_MODE);
        if (error == I2C_MSTR_NO_ERROR)
        {
            // Write address of register to be read
            error = I2C_MasterWriteByte(register_address);
            if (error == I2C_MSTR_NO_ERROR)
            {
                // Send restart condition
                error = I2C_MasterSendRestart(device_address, I2C_READ_XFER_MODE);
                if (error == I2C_MSTR_NO_ERROR)
                {
                    // Read data without acknowledgement
                    *data = I2C_MasterReadByte(I2C_NAK_DATA);
                }
            }
        }
        // Send stop condition
        I2C_MasterSendStop();
        // Return error code
        return error ? ERROR : NO_ERROR;
    }
    
     ErrorCode I2C_Peripheral_WriteRegister(uint8_t device_address,
                                            uint8_t register_address,
                                            uint8_t data)
    {
        // Send start condition
        uint8_t error = I2C_MasterSendStart(device_address, I2C_WRITE_XFER_MODE);
        if (error == I2C_MSTR_NO_ERROR)
        {
            // Write register address
            error = I2C_MasterWriteByte(register_address);
            if (error == I2C_MSTR_NO_ERROR)
            {
                // Write byte of interest
                error = I2C_MasterWriteByte(data);
            }
        }
        // Send stop condition
        I2C_MasterSendStop();
        // Return error code
        return error ? ERROR : NO_ERROR;
    }
    

/* [] END OF FILE */
