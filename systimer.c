/******************************************************************************
    INCLUDES
******************************************************************************/
#include <REGX52.H>

#define XTAL    (22118400ul)

#define TMS(X)  (((X)*(XTAL))/12000)

#define TH(X)   (((-(TMS(X))) >> 8)&0xFF)
#define TL(X)   (((-(TMS(X))))&0xFF)

#define BASE_TIME   (2)

/******************************************************************************
    VARIABLES
******************************************************************************/
unsigned long SystimerTime;

/******************************************************************************
    IMPLEMENTATION
******************************************************************************/
void    systimer_init(void)
{
    T2CON = 0;
    T2MOD = 0;
    
    TH2 = TH(BASE_TIME);
    TL2 = TL(BASE_TIME);
	RCAP2L = TL2;
	RCAP2H = TH2;
    TR2 = 1;

    SystimerTime = 0;
    ET2 = 1;
}

void    SystimerIRQ(void) interrupt TF2_VECTOR
{
    SystimerTime += BASE_TIME;
    TF2 = 0;
}

unsigned long   systimer_now(void)
{
unsigned long Temp;

    ET2 = 0;
    Temp = SystimerTime;
    ET2 = 1;

    return Temp;
}

unsigned long   systimer_elapsed(unsigned long Tempo)
{
unsigned long Temp;

    ET2 = 0;
    Temp = SystimerTime;
    ET2 = 1;

    if( Tempo <= Temp )
        return (Temp-Tempo);
    else
        return Temp+(0xFFFFFFFF-Tempo);

}