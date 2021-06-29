/*
  Serial.h
  
  Created: 28/06/2021
  Author: henrique.coser
  
  This example code is in the Public Domain
  This software is distributed on an "AS IS" BASIS, 
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, 
  either express or implied.
  
  Este c�digo de exemplo � de uso publico,
  Este software � distribuido na condi��o "COMO EST�",
  e N�O S�O APLIC�VEIS QUAISQUER GARANTIAS, implicitas 
  ou explicitas
*/

#ifndef _SERIAL_H_
#define _SERIAL_H_

void    UARTOpen();
void    UARTSend(char *ptBuffer, char BufferLen);
void    UARTSendString(const char *ptBuffer);

#endif
