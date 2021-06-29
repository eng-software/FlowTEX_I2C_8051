/*
  Serial.h
  
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

#ifndef _SERIAL_H_
#define _SERIAL_H_

void    UARTOpen();
void    UARTSend(char *ptBuffer, char BufferLen);
void    UARTSendString(const char *ptBuffer);

#endif
