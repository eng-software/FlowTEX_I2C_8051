#ifndef SERIAL_H
#define SERIAL_H

#define OPC_LEITURA_MEDIDOR     'L'

void    InitProtocolo(void);
void SendBuffer( unsigned char Opcode, unsigned int Tamanho, unsigned char *Buffer );

#endif
