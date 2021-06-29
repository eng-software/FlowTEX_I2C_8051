/*
  I2C.h
  
  Created: 28/06/2021
  Author: henrique.coser
  
  This example code is in the Public Domain
  This software is distributed on an "AS IS" BASIS, 
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, 
  either express or implied.
  
  Este código de exemplo é de uso publico,
  Este software é distribuido na condição "COMO ESTÁ",
  e NÃO SÃO APLICÁVEIS QUAISQUER GARANTIAS, implicitas 
  ou explicitas
*/

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
char I2CTransfer(char addr,  char *ptBufferOut, char nDataOut, char *ptBufferIn, char nDataIn);

#endif