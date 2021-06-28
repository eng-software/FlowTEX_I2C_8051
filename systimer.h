#ifndef SYSTIMER_H
#define SYSTIMER_H

void    systimer_init(void);
unsigned long   systimer_now(void);
unsigned long   systimer_elapsed(unsigned long Tempo);

#endif
