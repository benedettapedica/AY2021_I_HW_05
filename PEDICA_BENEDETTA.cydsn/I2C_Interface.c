/* ========================================
 *
 * File .c in which all the functions used for registers are defined.
 * \author Benedetta Pedica
 * \date: 14 November 2020
 *
 * ========================================
*/
#include "I2C_Interface.h" 
#include "I2C.h"

    ErrorCode I2C_Peripheral_Start(void) 
    {
        I2C_Start();  //start the I2C component
        return NO_ERROR;
    }
    
    
    ErrorCode I2C_Peripheral_Stop(void)
    {
        I2C_Stop();  //stop the I2C component
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
        uint8_t error = I2C_MasterSendStart(device_address,I2C_WRITE_XFER_MODE);
        if (error == I2C_MSTR_NO_ERROR)
        {
            error = I2C_MasterWriteByte(register_address); //write address of register to be read
            if (error == I2C_MSTR_NO_ERROR)
            {
                // restart condition: READ command in order to communicate to the device
                // the necessity to upload the needed data on the bus
                error = I2C_MasterSendRestart(device_address, I2C_READ_XFER_MODE);
                if (error == I2C_MSTR_NO_ERROR)
                {
                    //NACK signal to report ending of data transmission
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
    
     // Send start condition: WRITE command in order to communicate the device and the
     // register addresses to the slave peripheral
    {
        uint8_t error = I2C_MasterSendStart(device_address, I2C_WRITE_XFER_MODE);
        if (error == I2C_MSTR_NO_ERROR)
        {
            error = I2C_MasterWriteByte(register_address); //writing register adress
            if (error == I2C_MSTR_NO_ERROR)
            {
                error = I2C_MasterWriteByte(data); //writing byte of interest
            }
        }
        // Send stop condition
        I2C_MasterSendStop();
        // Return error code
        return error ? ERROR : NO_ERROR;
    }
    

/* [] END OF FILE */
