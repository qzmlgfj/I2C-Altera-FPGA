//#include "../Core_bsp/HAL/inc/alt_types.h"

#ifndef IIC_H_
#define IIC_H_
#define OUT 1 // The direction of Bidir
#define IN 0

void start(void);                              
void stop();                                   
void ack();                                    //Receive slave's acknowledge
void WriteI2C(alt_u8 dat);                     
alt_u8 ReadI2C();                              
void writeReg(alt_u16 addr, alt_u8 dat);       //Write 8 bits
alt_u8 readReg(alt_u16 addr);                  //Read 8 bits
void writeReg16Bit(alt_u16 addr, alt_u16 dat); //Write 16 bits
alt_u16 readReg16Bit(alt_u16 addr);            //Read 16 bits

#endif /*IICH*/
