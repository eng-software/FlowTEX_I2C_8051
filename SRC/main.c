#include <REGX52.H>
#include "systimer.h"
#include "serial.h"

//#define ENTRADAS_POR_IRQ 

#define cSTX         0x02



void    InitIRQs(void);

void    InitScanEntradas(void);
void    PrimeiroScanEntradas(void);
void    SegundoScanEntradas(void);
void    ScanEntradas(void);

unsigned long   Contador0;
unsigned long   Contador1;
unsigned char   Total[5];

bit EstadoEntrada0;
bit EstadoEntrada1;

bit UltimoEstadoEntrada0;
bit UltimoEstadoEntrada1;

void    (*ScanMState)(void);
unsigned long TimerScanEntradas;


void    InitScanEntradas(void)
{
    ScanMState           = PrimeiroScanEntradas; 
    UltimoEstadoEntrada0 = INT0; 
    UltimoEstadoEntrada1 = INT1;    
}


void    PrimeiroScanEntradas(void)
{
   EstadoEntrada0 = INT0; 
   EstadoEntrada1 = INT1;
   
   ScanMState = SegundoScanEntradas;
}

void    SegundoScanEntradas(void)
{
    if( EstadoEntrada0 == INT0)
    {
        if( UltimoEstadoEntrada0 != EstadoEntrada0 )
        {
            UltimoEstadoEntrada0 = EstadoEntrada0;
            Contador0 += 25;
        }
    }
    
    if( EstadoEntrada1 == INT1)
    {
        if( UltimoEstadoEntrada1 != EstadoEntrada1 )
        {
            UltimoEstadoEntrada1 = EstadoEntrada1;
            Contador1 += 25;
        }
    }

    ScanMState = PrimeiroScanEntradas; 
}

void    ScanEntradas(void)
{  
    if( systimer_elapsed(TimerScanEntradas) >= 10 )
    {
        TimerScanEntradas = systimer_now();
        ScanMState();
    }
}



void    main(void)
{   
unsigned long   Tempo;

    P1 = 0x55;
    
    systimer_init();
    InitIRQs();
    InitProtocolo();
    EA = 1;   
   
    P1 = 0xAA;
    InitScanEntradas();
    Tempo = systimer_now();
    while(1)
    {

#ifndef ENTRADAS_POR_IRQ        
        ScanEntradas();
#endif        
        
        if(systimer_elapsed(Tempo) >= 1000)
        {
            P3_7 = ~P3_7;
            Tempo = systimer_now();
            
            EA = 0;
            *((unsigned long *)&Total) = Contador0+Contador1;
            Contador0 = 0;
            Contador1 = 0;
            EA = 1;
            
            Total[4] = Total[0];    //Backp de 0 em 4
            Total[0] = Total[3];    //Coloca 3 em 0
            Total[3] = Total[4];    //Coloca o backup de 0 em 3
            Total[4] = Total[1];    //Backup de 1 em 4
            Total[1] = Total[2];    //Coloca 2 em 1
            Total[2] = Total[4];    //Coloca o backup de 1 em 2
                        
            SendBuffer( OPC_LEITURA_MEDIDOR, 4, Total );
        }       

    }
}

void    InitIRQs(void)
{
    

    INT0 = 1;
    INT1 = 1;
    
#ifdef  ENTRADAS_POR_IRQ        
    IT0 = 1;    //IRQ Externa 0 por borda
    IT1 = 1;    //IRQ Externa 1 por borda
    
    //Limpa IRQs
    IE0 = 0;
    IE1 = 0;
    
    Contador0 = 0;
    Contador1 = 0;
    
    EX0 = 1;
    EX1 = 1;
    PX0 = 1;
    PX1 = 1;
#endif    
}

void    IRQExterna0(void) interrupt 0 using 1
{
    IE0 = 0;
    Contador0++;
}

void    IRQExterna1(void) interrupt 2 using 2
{
    IE1 = 0;
    Contador1++;
}