#ifndef SERIAL_H
#define SERIAL_H

void    UARTOpen();
void    UARTSend(char *ptBuffer, char BufferLen);
void    UARTSendString(const char *ptBuffer);

#endif
