/* ========================================
 *
 * Header file where the Acceleration packet sent to the BCP is defined in detail.
 * \author Benedetta Pedica
 * \date: 14 November 2020
 *
 * ========================================
*/
#include "project.h"
#include "Macros.h"

//declaration of variables that will contain values coming from accelerometer
uint8_t Acc_X[ACC_SIZE];
uint8_t Acc_Y[ACC_SIZE];
uint8_t Acc_Z[ACC_SIZE];

//declaration of variables where information on acceleration will be stored
int16 A_X;
int16 A_Y;
int16 A_Z;

//auxiliary variables used to perform conversion from digits to m/s2
int16 A_X_u;
int16 A_Y_u;
int16 A_Z_u;

//float data needed to do the conversion
float A_X_Conv;
float A_Y_Conv;
float A_Z_Conv;

uint8_t Acceleration[TRANSMIT_SIZE]; //packet sent 

/* [] END OF FILE */
