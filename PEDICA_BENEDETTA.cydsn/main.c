/* ========================================
 *
 * Assignment 05 - AY 20-21.
 * In this project we test the accelerometer output
 * capabilities by sampling with high resolution 
 * all 3 axes at different frequencies (1-10-25-50-100-200 Hz).
 * Data is sent as m/s^2 (int).
 *
 * \author Benedetta Pedica
 * \date: 14 November 2020
 *
 * ========================================
*/
#include "project.h"
#include "I2C_Interface.h"
#include "InterruptRoutines.h"
#include "Macros.h"
#include "AccelerationPacket.h"

ErrorCode error; //error variable 

uint8 check; //variable used to check the Status register
uint8 freq;
volatile uint8 flag_button = 1; //initialized as status of button NOT pressed

int main(void)
{            
    EEPROM_Start();
    
    freq=EEPROM_ReadByte(EEPROM_FREQ_ADRESS); //reading value already present in EEPROM
    
    //if frequency is one of the allowed one, break;
    //otherwise, write the first frequency (1 Hz) 
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
    
    freq = EEPROM_ReadByte(EEPROM_FREQ_ADRESS); //check again value of frequency
    
    CyGlobalIntEnable;
    
    UART_Start();  
    I2C_Peripheral_Start();
    Clock_Deb_Start();
    CyDelay(5); //The boot procedure is complete about 5 milliseconds after device power-up
    
    char message[50] = {'\0'};  //buffer string to print out message in UART
    
    //check which devices are present on the I2C bus 
    for(uint8 i=0; i<128; i++)
    {
        if(I2C_IsDeviceConnected(i))
        {
            //print out the adress in Hex format
            sprintf(message, "Device 0x%02X is connected\r\n", i);
            UART_PutString(message); 
        }

    }
    
    //brief control on Control registers 1 and 4, if they are set up the accelerometer is ready to transmit data 
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS, 
                                             LIS3DH_CTRL_REG4, 
                                             CTRL_REG4_HR); //set the high resolution mode
        if (error == NO_ERROR)
        {
            //set the register with the value read from EEPROM
            error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS, LIS3DH_CTRL_REG1, freq); 
            if (error == NO_ERROR)
            {

                UART_PutString("Data in accelerometer are ready to be read");
            }
            else
            {
                UART_PutString("An error occurred during the setting of registers");   
            }
        }
  
   
   Button_isr_StartEx(Custom_Button_ISR); //initializing isr 

   //defining head and tail of the packet to be sent
   Acceleration[START] = HEAD;
   Acceleration[END] = TAIL;      

   for(;;)
    {
        if (flag_button == BUTTON_PRESSED)
        {            
            freq = EEPROM_ReadByte(EEPROM_FREQ_ADRESS); //read frequency

            switch(freq)
            {
                case CTRL_REG1_FREQ200:
                    EEPROM_UpdateTemperature();
                    EEPROM_WriteByte(CTRL_REG1_FREQ1, EEPROM_FREQ_ADRESS); //restart the cycle after 200 Hz 
                    break;
                default:
                    EEPROM_UpdateTemperature();
                    EEPROM_WriteByte(freq + FREQ_VARIATION, EEPROM_FREQ_ADRESS); //select the following frequency and update value in EEPROM
                    break;
            }

            freq = EEPROM_ReadByte(EEPROM_FREQ_ADRESS);
            
            //set the register again
            I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG1,
                                        freq);
            //set the flag to 1 to read acceleration data
            flag_button=1;
        }
        
      if (flag_button == 1) //button has not been pressed
        {
         //read the value of the Status register
         I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, 
                                        LIS3DH_STATUS_REG,
                                        &check);
          if (check & DATA_READY) //if a new data is avaliable
           {
            //Read the LSB and MSB acceleration for the three axes
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
                //ricomposition of value and 16 bit casting
                A_X_u = (int16)((Acc_X[LSB] | (Acc_X[MSB]<<8)))>>4;
                A_Y_u = (int16)((Acc_Y[LSB] | (Acc_Y[MSB]<<8)))>>4;
                A_Z_u = (int16)((Acc_Z[LSB] | (Acc_Z[MSB]<<8)))>>4;

                //X AXIS
                A_X_Conv = (A_X_u*M_digit_to_ms2)*1000; //from digit to m/s2
                A_X = (int16)(A_X_Conv); //from float to int
                Acceleration[AX_L] = A_X & 0xFF;
                Acceleration[AX_M] = A_X >> 8;
                
                //Y AXIS
                A_Y_Conv = (A_Y_u*M_digit_to_ms2)*1000; //from digit to m/s2
                A_Y = (int16)(A_Y_Conv);//from float to int
                Acceleration[AY_L] = (A_Y & 0xFF);
                Acceleration[AY_M] = (A_Y >> 8);

                //Z AXIS
                A_Z_Conv = (A_Z_u*M_digit_to_ms2)*1000; //from digit to m/s2
                A_Z = (int16)(A_Z_Conv);//from float to int
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
