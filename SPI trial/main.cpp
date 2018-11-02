/*
 * SPI trial.cpp
 *
 * Created: 2018/10/24 0:32:53
 * Author : leogr
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile char itrCount ;
//char oneSec = 249;
char tenHz = 125;

void SPI_MasterInit(void);
void SPI_MasterTransmit(char cData);
void timer0Setup();


int main(void)
{
	sei();
	SPI_MasterInit();
	timer0Setup();
	unsigned char ca = 0;
	unsigned char cb = 240;
	
	SPI_MasterTransmit('5');

    /* Replace with your application code */
    while (1) 
    {
		if (itrCount>2)
		{
			itrCount-=2;
			ca++;
			cb-=2;
			SPI_MasterTransmit(ca);
			SPI_MasterTransmit(cb);

		}
		
    }
}



// Given code from the document 
// On ATMEGA328P, PB5=sck PB4 = MISO PB3=MOSI PB2=SS

void SPI_MasterInit(void)
{
	/* Set MOSI and SCK output, all others input */
	DDRB = (1<<PORTB5) + (1<<PORTB3) + (1<<PORTB2) ;
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}


void SPI_MasterTransmit(char cData)
{
	// turn on ss 
	PORTB &= ~(1<<PORTB2);
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	
	// turn off SS
	PORTB |= 1<<PORTB2;
}

void timer0Setup()
{
	// default speed at 1Mhz

	TCCR0A = 0x02; // set WGM to mode 2 0b010 for CTC mode counter clear when TCNT0 match OCR0A
	
	TIMSK0 = 0x02; // enable interrupt on OCR0A
	TCCR0B = 4 ; // 16Mhz / 256 = 62500 Hz and start timer
	//62500 /50= 1.25 Khz
	OCR0A =	49 ;	// the value used to compare with timer.
	// when itrCounter reach 250, it means one second has passed
}

ISR(TIMER0_COMPA_vect) // interrupt occur when OCR0A match
{
	cli(); // disable interrupt
	++itrCount;
	sei();// enable  interrupt

}