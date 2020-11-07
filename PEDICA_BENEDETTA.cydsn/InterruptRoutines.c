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
#include "InterruptRoutines.h"
#include "I2C_Interface.h"

uint8_t frequency;

CY_ISR(Custom_Button_ISR)
{   
   frequency=EEPROM_ReadByte(0x0000);
   EEPROM_UpdateTemperature(); //security measure if temperature has changed of few degrees
  
   switch(frequency)
  {
      case CTRL_REG1_FREQ200: 
      EEPROM_WriteByte(CTRL_REG1_FREQ1, 0x0000);
    break;
    default:
      EEPROM_WriteByte(frequency+0x10, 0x0000);
    break;
  }

frequency=EEPROM_ReadByte(0x0000);

I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,LIS3DH_CTRL_REG1, frequency);
    
}

/* [] END OF FILE */
