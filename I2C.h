#ifndef _I2C_H_
#define _I2C_H_

void I2CInit();
void I2CStart();
void I2CRestart();
void I2CStop();
void I2CAck();
void I2CNak();
unsigned char I2CSend(unsigned char Data);
unsigned char I2CRead();
void I2CTransfer(char addr,  char *ptBufferOut, char nDataOut, char *ptBufferIn, char nDataIn);

#endif