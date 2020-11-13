/* ========================================
 *
 * Header file where error is defined.
 * \author Benedetta Pedica
 * \date: 14 November 2020
 *
 * ========================================
*/
#ifndef __ERRORCODES_H
    #define __ERRORCODES_H
    
    typedef enum 
    {
        NO_ERROR,           // No errors, comunication is succsessful
        ERROR               // Occurrence of an error, error message displayed through UART 
    } ErrorCode;

#endif

/* [] END OF FILE */
