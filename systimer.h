/*
  Systimer.h
  
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

#ifndef _SYSTIMER_H_
#define _SYSTIMER_H_

void    systimer_init(void);
unsigned long   systimer_now(void);
unsigned long   systimer_elapsed(unsigned long refTime);

#endif
