#include <avr/io.h>
#include <stdlib.h>
#include "pollserial.h"

#define BUFFER_SIZE 64

rbuffer rxbuffer = {0,0,0};

void USART_recieve() {
#if defined ( UDR0 )
	if( UCSR0A & _BV(RXC0)) {
		uint8_t i = (rxbuffer.head + 1) & (BUFFER_SIZE - 1);
		if ( i != rxbuffer.tail) {
			rxbuffer.buffer[rxbuffer.head] = UDR0;
			rxbuffer.head = i;
		}
	}
#else
	if( UCSRA & _BV(RXC)) {
		uint8_t i = (rxbuffer.head + 1) & (BUFFER_SIZE - 1);
		if ( i != rxbuffer.tail) {
			rxbuffer.buffer[rxbuffer.head] = UDR;
			rxbuffer.head = i;
		}
	}
#endif
}

pt2Funct pollserial::begin(long baud) {
	uint16_t baud_setting;
	bool use_u2x;
	
	rxbuffer.buffer = (unsigned char*)malloc(BUFFER_SIZE*sizeof(unsigned char));

	// U2X mode is needed for baud rates higher than (CPU Hz / 16)
	if (baud > F_CPU / 16) {
		use_u2x = true;
	}
	else {
		
		uint8_t nonu2x_baud_error = abs((int)(255-((F_CPU/(16*(((F_CPU/8/baud-1)/2)+1))*255)/baud)));
		uint8_t u2x_baud_error = abs((int)(255-((F_CPU/(8*(((F_CPU/4/baud-1)/2)+1))*255)/baud)));
    
		use_u2x = (nonu2x_baud_error > u2x_baud_error);
	}
	if (use_u2x) {
#if defined ( UDR0 )
		UCSR0A = _BV(U2X0);
#else
		UCSRA = _BV(U2X);
#endif
		baud_setting = (F_CPU / 4 / baud - 1) / 2;
	}
	else {
#if defined ( UDR0 )
		UCSR0A = 0;
#else
		UCSRA = 0;
#endif
		baud_setting = (F_CPU / 8 / baud - 1) / 2;
	}

#if defined ( UDR0 )
	UBRR0 = baud_setting;
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);
#else
	UBRR = baud_setting;
	UCSRB = _BV(RXEN) | _BV(TXEN);
#endif

	return &USART_recieve;
}

void pollserial::end() {
	UCSR0B &= ~(_BV(RXEN0) | _BV(TXEN0));
	free(rxbuffer.buffer);
}

uint8_t pollserial::available() {
	return (BUFFER_SIZE + rxbuffer.head - rxbuffer.tail) & (BUFFER_SIZE-1);
}

int pollserial::read() {
	if (rxbuffer.head == rxbuffer.tail)
		return -1;
	else {
		uint8_t c = rxbuffer.buffer[rxbuffer.tail];
		//tail = (tail + 1) & 63;
		if (rxbuffer.tail == BUFFER_SIZE)
			rxbuffer.tail = 0;
		else
			rxbuffer.tail++;
		return c;
	}
}

void pollserial::flush() {
	rxbuffer.head = rxbuffer.tail;
}

size_t pollserial::write(uint8_t c) { //void pollserial::write(uint8_t c) {
#if defined ( UDR0 )
	while (!((UCSR0A) & _BV(UDRE0)));
	UDR0 = c;
#else
	while (!((UCSRA) & _BV(UDRE)));
	UDR = c;
#endif
	return c;
}