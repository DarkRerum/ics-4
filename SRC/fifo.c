#include "fifo.h"
#include "led.h"

void initFifo( fifo_t* pFifo ) {
    pFifo->size = 0;
    pFifo->head = 0;
}

unsigned char pullElement( fifo_t* pFifo ) {
    unsigned char element = pFifo->elems[pFifo->head];
    if (pFifo->size == 0) {
        return 0;
    }
    pFifo->head = (pFifo->head + 1) % (FIFO_MAX_SIZE - 1);
    pFifo->size--;
    return element;
}

static unsigned char getTail( const fifo_t* pFifo ) {
    return (pFifo->head + pFifo->size) % (FIFO_MAX_SIZE - 1);
}

void pushElement( fifo_t* pFifo, unsigned char element ) {
    unsigned char tail = getTail(pFifo);
    pFifo->elems[tail] = element;	
    pFifo->size++;
	if (pFifo->size > FIFO_MAX_SIZE) {
		pFifo->size = FIFO_MAX_SIZE;
	}
}

int isEmpty( fifo_t* pFifo ) {
	//leds(pFifo->size);
    return pFifo->size == 0;
}