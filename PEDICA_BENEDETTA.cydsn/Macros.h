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

//adress of all LISDH3 registers used for this application
#define LIS3DH_DEVICE_ADDRESS 0x18  //address of the slave device

#define LIS3DH_STATUS_REG    0x27
#define LIS3DH_CTRL_REG0     0x1E
#define LIS3DH_CTRL_REG1     0x20
#define LIS3DH_CTRL_REG4     0x23
#define CTRL_REG4_HR         0x08
#define LIS3DH_CTRL_REG1_ODR 0x57


//adress of registers used to store value of acceleration in the three axes
//divided in low and high
#define LIS3DH_OUT_X_L 0x28
#define LIS3DH_OUT_X_H 0x29
#define LIS3DH_OUT_Y_L 0x2A
#define LIS3DH_OUT_Y_H 0x2B
#define LIS3DH_OUT_Z_L 0x2C
#define LIS3DH_OUT_Z_H 0x2D 


//address of registers for data rate configuration in CTRL_REG_1
#define CTRL_REG1_FREQ1   0x17
#define CTRL_REG1_FREQ10  0x27
#define CTRL_REG1_FREQ25  0x37
#define CTRL_REG1_FREQ50  0x47
#define CTRL_REG1_FREQ100 0x57
#define CTRL_REG1_FREQ200 0x67


//adress of EEPROM register used to store different values of frequency
 #define EEPROM_FREQ_ADRESS 0x0000


//variation of frequency used to cycle through the 6 different values
#define FREQ_VARIATION 0x10 

//macros used for the CONVERSION from digits to m/s2
#define gravity 9.81
#define M_digit_to_ms2 4*gravity/4095


//list of macros used to define the data packet that is sent through UART
//6 bytes are used for acceleration data (2 bytes per axis, one least significant bit and the other most significant bit)
//first and last one are used for header and tail respectively
#define MSB 1
#define LSB 0
#define HEAD 0xA0
#define TAIL 0xC0
#define TRANSMIT_SIZE 1+ACC_BYTES+1
#define ACC_BYTES 6
#define START 0
#define AX_L 1
#define AX_M 2    
#define AY_L 3
#define AY_M 4
#define AZ_L 5
#define AZ_M 6
#define END TRANSMIT_SIZE-1
#define ACC_SIZE 2 


//this is done to check on the status register: if a new value is stored,
//new values of acceleration are ready to be sent
#define DATA_READY 0b00001000 


/* [] END OF FILE */
