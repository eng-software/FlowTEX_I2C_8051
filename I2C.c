/*
  I2C.c
  
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

/*-----------------------------------------------------------------------------
    DEFINES
-----------------------------------------------------------------------------*/

//SDA and SCL pin setup
#define SDA     P0_0
#define SCL     P0_1

/*-----------------------------------------------------------------------------
    IMPLEMENTATION
-----------------------------------------------------------------------------*/

/*
    I2C Pin initialization
*/
void I2CInit()
{
    SDA = 1;
    SCL = 1;
}

/*
    I2C Start condition
*/
void I2CStart()
{
    SDA = 0;
    SCL = 0;
}
 
/*
    I2C Restart condition
*/
void I2CRestart()
{
    SDA = 1;
    SCL = 1;
    SDA = 0;
    SCL = 0;
}
 
/*
    I2C Stop condition
*/
void I2CStop()
{
    SCL = 0;
    SDA = 0;
    SCL = 1;
    SDA = 1;
}
 
/*
    Send ACK
*/
void I2CAck()
{
    SDA = 0;
    SCL = 1;
    SCL = 0;
    SDA = 1;
}
 
/*
    Send NACK
*/
void I2CNak()
{
    SDA = 1;
    SCL = 1;
    SCL = 0;
    SDA = 1;
}
 
/*
    Send one byte through I2C protocol
    
    Returns 0 if ACK  , 1 if NACK
*/
unsigned char I2CSend(unsigned char Data)
{
    unsigned char i, ack_bit;
    
    for (i = 0; i < 8; i++) 
    {
        if ((Data & 0x80) == 0)
        {   SDA = 0;   }
        else
        {   SDA = 1;    }
        
        SCL = 1;
        SCL = 0;
        Data<<=1;
    }
    
    SDA = 1;
    SCL = 1;
    ack_bit = SDA;
    SCL = 0;
    return ack_bit;
}
 
/*
    Receive one byte through I2C protocol
*/
unsigned char I2CRead()
{
    unsigned char i, Data=0;
    
    for (i = 0; i < 8; i++) 
    {
        SCL = 1;
        if(SDA)
        {   Data |=1;   }
        if(i<7)
        {   Data<<=1;   }        
        SCL = 0;
    }
    return Data;
}

/*
    Transfer data through I2C port
    
    Send a write buffer and a read buffer.
    Nothing is written if ptBufferOut is NULL or nDataOut is 0
    Nothing is read if ptBufferIn is NULL or nDataIn is 0

    Return 0 if ACK , 1 if NACK
*/
char I2CTransfer(char addr,  char *ptBufferOut, char nDataOut, char *ptBufferIn, char nDataIn)
{
    char ack = 1;
    
    //Shift address to add write/read bit
    addr = addr << 1;
        
    if((ptBufferOut)&&(nDataOut))
    {
        /*****************************************
        * Write to slave device        
        *****************************************/
        /* Init i2c ports first */
        I2CInit();
        /* Send start condition */
        I2CStart();
        /* Send slave address */
        ack = I2CSend(addr);
        /*
         * ack == 1 => NAK
         * ack == 0 => ACK
        */
        while((nDataOut--)&&(!ack))
        {        
            ack = I2CSend(*ptBufferOut++);            
        }
        
        /* Send stop condition */
        I2CStop();
    }
    
    if((ptBufferIn)&&(nDataIn))
    {  
        //Set readd bit  
        addr |= 0x01;
        /*****************************************
        * Read from slave device    
        *****************************************/
        /* Init i2c ports first - Should be done once in main */
        I2CInit();
        /* Send start condition */
        I2CStart();
        /*
        * Send slave address with Read bit set        
        */
        ack = I2CSend(addr);
        
        if(!ack)
        {
            while(nDataIn)
            {
                *ptBufferIn++ = I2CRead();                
                nDataIn--;
                
                if(nDataIn != 0)
                {    
                    /* Send ack */
                    I2CAck();
                }
            }
        }

        /*
         * Send nak for last byte to indicate
         * End of transmission
        */
        I2CNak();
        /* Send stop condition */
        I2CStop();
    }
    
    
    return ack;    
}