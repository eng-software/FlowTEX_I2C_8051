/*
  main.c
  
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
#include "serial.h"
#include "I2C.h"
#include <stdio.h>
#include "systimer.h"

/*-----------------------------------------------------------------------------
    DEFINES
-----------------------------------------------------------------------------*/
#define FLOW_TEX_ADDRESS    0x20

/*-----------------------------------------------------------------------------
    TYPEDEFS
-----------------------------------------------------------------------------*/

//FlowTex register table
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

//Union to manipulate long as a buffer
typedef union tLong
{
    long value;
    unsigned char bytes[4]; 
}tLong;

typedef union tShort
{
    short value;
    unsigned char bytes[2]; 
}tShort;

/*-----------------------------------------------------------------------------
    IMPLEMENTATION
-----------------------------------------------------------------------------*/

void    main(void)
{        
    tLong Flow;
    tLong FullScale;
    tShort Temperature;
    
    float fFlow;
    char bufferSerial[15];
	unsigned long refreshTimer;    
    tI2CTable FlowTexData;
    char dataAddress;
    
    //Init systimer
    systimer_init();   
    
    //Open UART port
    UARTOpen();
    
    //Enable interrupts
    EA = 1;
    
    //Update refresh timer
	refreshTimer = systimer_now();
    
    while(1)
    {
		//Every 500ms will read and print data read from FlowTEX
        if(systimer_elapsed(refreshTimer) >= 500)
		{
            //Will read all I2C table, starting from address 0
            dataAddress = 0;
			if(I2CTransfer(FLOW_TEX_ADDRESS, &dataAddress, 1, (char *)&FlowTexData, sizeof(FlowTexData)) == 0)
			{
                //-----------------------------------------------
                //FlowTEX is LittleEndian and 8051 is BigEndian
                //It will swap the bytes
                Flow.bytes[1] = FlowTexData.flow[2];
                Flow.bytes[2] = FlowTexData.flow[1];
                Flow.bytes[3] = FlowTexData.flow[0];        
                
                //Since is 24bits the MSB indicates a negative value
                //If negative value the first byte must be FF
                if(Flow.bytes[1]&0x80)
                {   Flow.bytes[0] = 0xFF;  }
                else
                {   Flow.bytes[0] = 0x00;  }
                //-----------------------------------------------
                
                //-----------------------------------------------
                //FlowTEX is LittleEndian and 8051 is BigEndian
                //It will swap the bytes
                FullScale.bytes[0] = 0;
                FullScale.bytes[1] = FlowTexData.fullScale[2];
                FullScale.bytes[2] = FlowTexData.fullScale[1];
                FullScale.bytes[3] = FlowTexData.fullScale[0];
                //-----------------------------------------------
                
                //Convert 24bits value to Sccm   
                fFlow = (float)FullScale.value * ((float)Flow.value/(float)0x7FFFFF);
                  
                //-----------------------------------------------
                //Get temperature value 
                //FlowTEX is LittleEndian and 8051 is BigEndian
                //It will swap the bytes
                Temperature.bytes[0] = FlowTexData.temp[1];
                Temperature.bytes[1] = FlowTexData.temp[0];
                //-----------------------------------------------
                
                //Print Flow
                UARTSendString("\r\n Flow [Sccm]: ");			
                sprintf(bufferSerial,"%11.3f", fFlow);
                UARTSendString(bufferSerial);
                
                //Print Full Scale
                UARTSendString("\r\n Full Scale [Sccm]: ");
                sprintf(bufferSerial,"%ld", FullScale.value);
                UARTSendString(bufferSerial);
                
                //Temperature
                UARTSendString("\r\n Temperature [°C]: ");
                sprintf(bufferSerial,"%6.2f", ((float)Temperature.value)/100.0f);
                UARTSendString(bufferSerial);
                                
                //Print Serial Number
                UARTSendString("\r\n Serial Number: ");
                UARTSendString(FlowTexData.serialNumber); 
                
                //Print Version
                UARTSendString("\r\n Version: ");
                sprintf(bufferSerial,"%bu.%bu.%bu.%bu", FlowTexData.version[0], FlowTexData.version[1],
                    FlowTexData.version[2], FlowTexData.version[3]);
                UARTSendString(bufferSerial);
                
                UARTSendString("\r\n");
            }
            else
            {
                UARTSendString("\r\n DEVICE NOT RESPONDING! \r\n");
            }
			refreshTimer = systimer_now();
		}
	}
}


