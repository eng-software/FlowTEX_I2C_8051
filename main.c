#include <REGX52.H>
#include "serial.h"
#include "I2C.h"
#include <stdio.h>


#define SWAP(X,Y)   X = X^Y;     /* XOR the values and store the result in X */ \
                    Y = Y^X;     /* XOR the values and store the result in Y */ \
                    X = X^Y      /* XOR the values and store the result in X */

typedef struct tI2CTable
{                                //Address range
    char flow[3];                //0..2
    char flowChks;               //3
    char temp[2];                //4..5
    char tempChks;               //6
    char fullScale[3];           //7..9
    char fullScaleChks;          //10
    char serialNumber[10];       //11..20
    char serialNumberChks;       //21
    char version[4];             //22..25
    char versionChks;            //26
    char fwChks[4];              //27..30
    char fwChksChks;             //31
}tI2CTable;

typedef union tLong
{
    long value;
    unsigned char bytes[4]; 
}tLong;

void    main(void)
{
    tLong Flow;
    tLong FullScale;
    float fFlow;
    char bufferSerial[15];
    
    tI2CTable FlowTexData;
    
        
    UARTOpen();
    
    while(1)
    {
        I2CTransfer(0x20, "\0", 1, (char *)&FlowTexData, sizeof(FlowTexData));
        
        SWAP(FlowTexData.flow[0], FlowTexData.flow[2]);
        
        
        Flow.bytes[1] = FlowTexData.flow[2];
        Flow.bytes[2] = FlowTexData.flow[1];
        Flow.bytes[3] = FlowTexData.flow[0];        
        if(Flow.bytes[1]&0x80)
        {   Flow.bytes[0] = 0;  }
        else
        {   Flow.bytes[0] = 0xFF;  }
        
        
        FullScale.bytes[0] = 0;
        FullScale.bytes[1] = FlowTexData.fullScale[2];
        FullScale.bytes[2] = FlowTexData.fullScale[1];
        FullScale.bytes[3] = FlowTexData.fullScale[0];
        
        fFlow = (float)FullScale.value * ((float)Flow.value/(float)0x7FFFFF);
        
        //Print Flow
        UARTSendString("\r\n Flow (Sccm): ");
        sprintf(bufferSerial,"%.3f", fFlow);
        UARTSendString(bufferSerial);
        
        //Print Serial Number
        UARTSendString("\r\n");
        UARTSendString(FlowTexData.serialNumber); 
    }
}


