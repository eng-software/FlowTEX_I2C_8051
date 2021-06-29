/*
  Systimer.c
  
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

/*
    Systimer tick in ms
    
    Every tick time the timer variable will be incremented
*/   
#define BASE_TIME   (2)


//XTAL Frequency
#define XTAL    (22118400ul)

//Convert milliseconds to machine cycles
#define TMS(X)  (((X)*(XTAL))/12000)

//Calculate timer registrer TH and TL based on desired ms
#define TH(X)   (((-(TMS(X))) >> 8)&0xFF)
#define TL(X)   (((-(TMS(X))))&0xFF)


/*-----------------------------------------------------------------------------
    INCLUDES
-----------------------------------------------------------------------------*/
unsigned long SystimerTime;

/*-----------------------------------------------------------------------------
    IMPLEMENTATION
-----------------------------------------------------------------------------*/


/*
    Systimer initialization.
    
    Configure TIMER2 to giver the BASE_TIME cadence
*/
void    systimer_init(void)
{    
    //Stop timer and set it to auto reload
    T2CON = 0;
    T2MOD = 0;
    
    //Configure timer registers converting ms from BASE_TIME to
    //machine cycles
    TH2 = TH(BASE_TIME);
    TL2 = TL(BASE_TIME);
	RCAP2L = TL2;
	RCAP2H = TH2;
    
    //Start timer
    TR2 = 1;

    SystimerTime = 0;
    
    //Enable TIMER2 IRQ
    ET2 = 1;
}

/*
    TIMER2 IRQ
    
    Increment system time based on BASE_TIME value
*/
void    SystimerIRQ(void) interrupt TF2_VECTOR
{
    SystimerTime += BASE_TIME;
    TF2 = 0;
}

/*
    Get current time
    
    Get the current time value to be used as a reference time
*/
unsigned long   systimer_now(void)
{
unsigned long Temp;

    //Disable timer during variable copy
    //This ensure that variable will not be change during memmory copy
    ET2 = 0;
    Temp = SystimerTime;
    ET2 = 1;

    return Temp;
}

/*
    Get elapsed time from a given refference
    
    Calculate the time elapsed calculating the difference between reference time and
    current time
*/
unsigned long   systimer_elapsed(unsigned long refTime)
{
unsigned long Temp;

    //Disable timer during variable copy
    //This ensure that variable will not be change during memmory copy
    ET2 = 0;
    Temp = SystimerTime;
    ET2 = 1;

    if( refTime <= Temp )
        return (Temp-refTime);
    else
        return Temp+(0xFFFFFFFF-refTime);

}