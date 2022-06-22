#ifndef PSERIAL_H
#define PSERIAL_H

#include <inttypes.h>
#include "Print.h"

typedef struct {
	uint8_t head;
	uint8_t tail;
	unsigned char * buffer;
} rbuffer;

typedef void (*pt2Funct)();

class pollserial : public Print {
	public:
		pt2Funct begin(long);
		void end();
		uint8_t available(void);
		int read(void);
		void flush(void);
		virtual size_t write(uint8_t);
		using Print::write; 
};

void USART_recieve();
#endif