#include <REGX52.H>
#include "systimer.h"

#define XTAL        (22118400ul)
#define BAUDRATE    (19200ul)
#define TBAUD       (256 - ((XTAL) / (32ul*12ul*(BAUDRATE))))

void    UARTOpen()
{
    SCON = 0x50;    //8bits variable baudrate    
    
    TMOD &= 0x0F;
    TMOD |= 0x20;
    TH1 = TBAUD;
    TL1 = TBAUD;
    TR1 = 1;    
    ES = 0;
}

void    UARTSend(const char *ptBuffer, char BufferLen)
{
    while(BufferLen--)
    {
        TI = 0;
        SBUF =  *ptBuffer++;
        while(TI == 0);
    }
}


void    UARTSendString(const char *ptBuffer)
{
    while(*ptBuffer != 0)
    {
        TI = 0;
        SBUF =  *ptBuffer++;
        while(TI == 0);
    }
}
