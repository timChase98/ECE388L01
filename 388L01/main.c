
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int timermode = 0;
int times[] = {-6250, -3125, -31250};

ISR(TIMER1_OVF_vect)
{
	TCNT1 = times[timermode];	// set frequency
	PORTB ^= (1 << PORTB5);		// toggle LED on overflow
}


int main(void)
{
	DDRB = (1 << PORTB5);		// set LED to output
	TCNT1 = -1;					// let timer quickly overflow (set to proper freq)
	
	TCCR1A = 0;
	TCCR1B = (0b100 << CS10);	// prescale 256
	TIMSK1 = 1 << TOIE1;		// overflow timer
	sei();
	
	while(1)
	{
		if( (PINB & (1 << PINB7)) == 0)			// pin 7 pressed?
		{
			_delay_ms(1);						// prevent switch bouncing
			while( (PINB & (1 << PINB7)) == 0)	// debounce (do nothing until button is released, only works with single button)
				;
			timermode = (timermode + 1) % 3;	// change active frequency
			TCNT1 = -1;							// cause overflow, skip rest of current cycle
			_delay_ms(1);
		}
	}
}

