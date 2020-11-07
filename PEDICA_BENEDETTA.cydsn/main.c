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
#include "stdio.h"
#include "InterruptRoutines.h"


#define LIS3DH_STATUS_REG 0x27
#define LIS3DH_CTRL_REG0  0x1E

#define LIS3DH_CTRL_REG4  0x23

#define CTRL_REG4_HR 0x08

#define LIS3DH_CTRL_REG1_ODR 0x57

#define LIS3DH_OUT_X_L 0x28
#define LIS3DH_OUT_X_H 0x29
#define LIS3DH_OUT_Y_L 0x2A
#define LIS3DH_OUT_Y_H 0x2B
#define LIS3DH_OUT_Z_L 0x2C
#define LIS3DH_OUT_Z_H 0x2D 

#define gravity 9.81

#define M_digit_to_ms2 4*gravity/4096
//#define Q_digit_to_ms2 M_digit_to_ms2*4095


ErrorCode error;
uint8_t Acc_X[2];
uint8_t Acc_Y[2];
uint8_t Acc_Z[2];
int16 A_X;
int16 A_Y;
int16 A_Z;
int16 A_X_u;
int16 A_Y_u;
int16 A_Z_u;
float A_X_Conv;
float A_Y_Conv;
float A_Z_Conv;

uint8 value;

int main(void)
{
      
    I2C_Peripheral_Start();
    UART_Start();
    Clock_Deb_Start();
       
    CyGlobalIntEnable; 
  
    CyDelay(5);
    
    EEPROM_Start();
    //EEPROM_UpdateTemperature();
    //EEPROM_WriteByte(1,0x0000);
    
    value=EEPROM_ReadByte(0x0000);
    
    switch (value)
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
        EEPROM_WriteByte(CTRL_REG1_FREQ1,0x0000);
        break;    
           
    }

    char message[50] = {'\0'};
    
    for(uint8 i=0; i<128; i++)
    {
        if(I2C_IsDeviceConnected(i))
        {
            sprintf(message, "Device 0x%02X is connected\r\n", i);
            UART_PutString(message); 
        }

    }
    
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG4,
                                        CTRL_REG4_HR);    
    
    uint8_t ctrl_reg4;
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG4,
                                        &ctrl_reg4);
    
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
       
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG1,
                                        LIS3DH_CTRL_REG1_ODR);
    uint8_t ctrl_reg1;
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG1,
                                        &ctrl_reg1);
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
    
   uint8 check;
   Button_isr_StartEx(Custom_Button_ISR);
    
   for(;;)
    {
        I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_STATUS_REG,
                                        &check);
        if (check & 0b00001000)
        {
        
            error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                    LIS3DH_OUT_X_L,
                                                    &Acc_X[0]);
            error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                    LIS3DH_OUT_X_H,
                                                    &Acc_X[1]);
           
            error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                    LIS3DH_OUT_Y_L,
                                                    &Acc_Y[0]);
            
            error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                    LIS3DH_OUT_Y_H,
                                                    &Acc_Y[1]);
            
            error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                    LIS3DH_OUT_Z_L,
                                                    &Acc_Z[0]);
            
            error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                    LIS3DH_OUT_Z_H,
                                                    &Acc_Z[1]);
                
              
            uint8_t header = 0xA0;
            uint8_t footer = 0xC0;
            uint8_t Acceleration[8];
            Acceleration[0]=header;
            Acceleration[7]=footer;
                            
            if (error==NO_ERROR)
            {
                 A_X_u =(int16_t)((Acc_X[0] | (Acc_X[1]<<8)))>>4;
                 A_Y_u =(int16_t)((Acc_Y[0] | (Acc_Y[1]<<8)))>>4;
                 A_Z_u =(int16_t)((Acc_Z[0] | (Acc_Z[1]<<8)))>>4;
            
                /*if (A_X_u>= 0 && A_X_u <=2047)
                {
                  A_X_Conv = (A_X*M_digit_to_ms2)*1000;
                }
                
                else if (A_X_u >2047 && A_X_u <4095)
                {
                  A_X_Conv = (A_X_u*M_digit_to_ms2-Q_digit_to_ms2)*1000; 
                }
                */
                A_X_Conv= (A_X_u*M_digit_to_ms2)*1000;
                A_X= (int16) (A_X_Conv);
                Acceleration[1] = A_X & 0xFF;
                Acceleration[2] = A_X >>8;
                
                 /*
                if (A_Y_u >= 0 && A_Y_u <=2047)
                {
                  A_Y_Conv= (A_Y_u*M_digit_to_ms2)*1000;
                }
                
                else if (A_Y_u >2047 && A_Y_u <4096)
                {
                  A_Y_Conv= (A_Y_u*M_digit_to_ms2-Q_digit_to_ms2)*1000; 
                }
                */
                A_Y_Conv= (A_Y_u*M_digit_to_ms2)*1000;
                A_Y = (int16) (A_Y_Conv);
                Acceleration[3] = A_Y & 0xFF;
                Acceleration[4] = A_Y >>8;
                
                /*
                if (A_Z_u >= 0 && A_Z_u <=2047)
                {
                  A_Z_Conv= (A_Z_u*M_digit_to_ms2)*1000;
                }
                
                else if (A_Z_u >2047 && A_Z_u <4096)
                {
                  A_Z_Conv= (A_Z_u*M_digit_to_ms2-Q_digit_to_ms2)*1000; 
                }
                */
                A_Z_Conv= (A_Z_u*M_digit_to_ms2)*1000;
                A_Z = (int16) (A_Z_Conv);
                Acceleration[5] = A_Z & 0xFF;
                Acceleration[6] = A_Z >>8;
             
                UART_PutArray(Acceleration,8);
            
            }
        
      }
   }
}

 
/* [] END OF FILE */
