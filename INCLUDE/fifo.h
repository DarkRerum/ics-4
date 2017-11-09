#ifndef __FIFO__H
#define __FIFO__H

#define FIFO_MAX_SIZE 30

typedef struct fifo_t {
    unsigned char size;
    unsigned char head;
    unsigned char elems[FIFO_MAX_SIZE];
} fifo_t;

void initFifo( fifo_t* pFifo );
unsigned char pullElement( fifo_t* pFifo );
void pushElement( fifo_t* pFifo, unsigned char element );
int isEmpty( fifo_t* pFifo );

#endif //__FIFO__H