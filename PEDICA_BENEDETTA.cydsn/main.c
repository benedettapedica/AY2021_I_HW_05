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
#include "project.h"
#include "I2C_Interface.h"
#include "InterruptRoutines.h"
#include "Macros.h"

ErrorCode error;

uint8_t Acc_X[ACC_SIZE];
uint8_t Acc_Y[ACC_SIZE];
uint8_t Acc_Z[ACC_SIZE];

int16 A_X;
int16 A_Y;
int16 A_Z;

int16 A_X_u;
int16 A_Y_u;
int16 A_Z_u;

float A_X_Conv;
float A_Y_Conv;
float A_Z_Conv;

uint8_t Acceleration[TRANSMIT_SIZE];

uint8 check;
uint8 freq;
volatile uint8 flag_button = 1;

int main(void)
{            
    EEPROM_Start();
    
    freq=EEPROM_ReadByte(EEPROM_FREQ_ADRESS);
       
    switch (freq)
    {
        case CTRL_REG1_FREQ1:
        case CTRL_REG1_FREQ10:
        case CTRL_REG1_FREQ25:
        case CTRL_REG1_FREQ50:
        case CTRL_REG1_FREQ100:
        case CTRL_REG1_FREQ200:
        break;
                
        default:
        EEPROM_UpdateTemperature();
        EEPROM_WriteByte(CTRL_REG1_FREQ1,EEPROM_FREQ_ADRESS);
        break;    
           
    }
    
    freq = EEPROM_ReadByte(EEPROM_FREQ_ADRESS);
    
    CyGlobalIntEnable;
    
    UART_Start();  
    I2C_Peripheral_Start();
    CyDelay(5);

    char message[50] = {'\0'};
    
    for(uint8 i=0; i<128; i++)
    {
        if(I2C_IsDeviceConnected(i))
        {
            sprintf(message, "Device 0x%02X is connected\r\n", i);
            UART_PutString(message); 
        }

    }
    
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS, LIS3DH_CTRL_REG4, CTRL_REG4_HR);
        if (error == NO_ERROR)
        {
            error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS, LIS3DH_CTRL_REG1, freq);
            if (error == NO_ERROR)
            {

                UART_PutString("ACCELEROMETER READY");
            }
            else
            {
                UART_PutString("ERROR OCCURRED DURING REGISTER SETTING");   
            }
        }
        
   /* if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 4 written as: 0x%02X\r\n", CTRL_REG4_HR);
        UART_PutString(message); 
    }
    else
    {
        UART_PutString("Error \r\n");   
    }
    */
       
   
    /*
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 4 written as: 0x%02X\r\n", LIS3DH_CTRL_REG1_ODR);
        UART_PutString(message); 
    }
    else
    {
        UART_PutString("Error\r\n");   
    }
    */
    
   Clock_Deb_Start();
   Button_isr_StartEx(Custom_Button_ISR);

   Acceleration[START] = HEAD;
   Acceleration[END] = TAIL;      

   for(;;)
    {
        if (flag_button == BUTTON_PRESSED)
        {            
            freq = EEPROM_ReadByte(EEPROM_FREQ_ADRESS);

            switch(freq)
            {
                case CTRL_REG1_FREQ200:
                    EEPROM_UpdateTemperature();
                    EEPROM_WriteByte(CTRL_REG1_FREQ1, EEPROM_FREQ_ADRESS);
                    break;
                default:
                    EEPROM_UpdateTemperature();
                    EEPROM_WriteByte(freq + FREQ_VARIATION, EEPROM_FREQ_ADRESS);
                    break;
            }

            freq = EEPROM_ReadByte(EEPROM_FREQ_ADRESS);
            
            
            I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG1,
                                        freq);
            flag_button=1;
        }
        
      if (flag_button == 1)
        {
         I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_STATUS_REG,
                                        &check);
          if (check & DATA_READY)
           {
            error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, LIS3DH_OUT_X_L, &Acc_X[LSB]);
            if (error == NO_ERROR)
             {
                error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, LIS3DH_OUT_X_H, &Acc_X[MSB]);
                if (error == NO_ERROR)
                 {
                    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, LIS3DH_OUT_Y_L, &Acc_Y[LSB]);
                    if (error == NO_ERROR)
                     {
                        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, LIS3DH_OUT_Y_H, &Acc_Y[MSB]);
                        if (error == NO_ERROR)
                         {
                            error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, LIS3DH_OUT_Z_L, &Acc_Z[LSB]);
                            if (error == NO_ERROR)
                             {
                                error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, LIS3DH_OUT_Z_H, &Acc_Z[MSB]);
                             }
                         }
                     }
                 }
             }
     
           if (error==NO_ERROR)
             {
                A_X_u = (int16)((Acc_X[LSB] | (Acc_X[MSB]<<8)))>>4;
                A_Y_u = (int16)((Acc_Y[LSB] | (Acc_Y[MSB]<<8)))>>4;
                A_Z_u = (int16)((Acc_Z[LSB] | (Acc_Z[MSB]<<8)))>>4;

                A_X_Conv = (A_X_u*M_digit_to_ms2)*1000;
                A_X = (int16)(A_X_Conv);

                Acceleration[AX_L] = A_X & 0xFF;
                Acceleration[AX_M] = A_X >> 8;
                
                A_Y_Conv = (A_Y_u*M_digit_to_ms2)*1000;
                A_Y = (int16)(A_Y_Conv);

                Acceleration[AY_L] = (A_Y & 0xFF);
                Acceleration[AY_M] = (A_Y >> 8);


                A_Z_Conv = (A_Z_u*M_digit_to_ms2)*1000;
                A_Z = (int16)(A_Z_Conv);

                Acceleration[AZ_L] = (A_Z & 0xFF);
                Acceleration[AZ_M] = (A_Z >> 8);
                
                UART_PutArray(Acceleration,TRANSMIT_SIZE);
                
                }
           else 
               {
                 UART_PutString("An error occurred, check acceleration registers \r\n"); 
               }
          }
      }     
   }
}

 
/* [] END OF FILE */
