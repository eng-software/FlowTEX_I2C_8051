/*
  Serial.c
  
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

/*-----------------------------------------------------------------------------
    INCLUDES
-----------------------------------------------------------------------------*/
#include <REGX52.H>
#include "systimer.h"

/*-----------------------------------------------------------------------------
    DEFINES
-----------------------------------------------------------------------------*/

//XTAL in Hz
#define XTAL        (22118400ul)

//Desired baudrate
#define BAUDRATE    (19200ul)

//Baudrate calculation for TIMER1
#define TBAUD       (256 - ((XTAL) / (32ul*12ul*(BAUDRATE))))


/*-----------------------------------------------------------------------------
    IMPLEMENTATION
-----------------------------------------------------------------------------*/

/*
    Open UART Port
    
    Configure TIMER1 to be baudrate generator
    Configure baudrate base on BAUDRATE definition
*/
void    UARTOpen()
{
    //8bits variable baudrate     
    SCON = 0x50;    
    
    //Auto reload timer 1
    TMOD &= 0x0F;   
    TMOD |= 0x20;
    
    //Configure TIMER based on BAUDRATE definition
    TH1 = TBAUD;
    TL1 = TBAUD;
    
    //Enable tranmition
    TR1 = 1;    
    
    //Will not use IRQ
    ES = 0;
}

/*
    Send a buffer through UART Port
    
    Send a buffer with specific size through serial port
    This will not use IRQ, this function only returns ]
    after the end of transmition
*/
void    UARTSend(const char *ptBuffer, char BufferLen)
{
    while(BufferLen--)
    {
        //Clear TI flag
        TI = 0;
        
        //Put data on buffer        
        SBUF =  *ptBuffer++;
        
        //Wait transmition end
        while(TI == 0);
    }
}

/*
    Send a string through UART Port
    
    Send a null terminate string  through serial port
    This will not use IRQ, this function only returns ]
    after the end of transmition
*/
void    UARTSendString(const char *ptBuffer)
{
    while(*ptBuffer != 0)
    {
        TI = 0;
        SBUF =  *ptBuffer++;
        while(TI == 0);
    }
}
