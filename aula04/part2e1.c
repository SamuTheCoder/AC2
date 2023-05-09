#include <detpic32.h>

#define K (PBCLK / 1000)

void delay(unsigned int ms);

int main(void){
	
	char c;

	TRISB = TRISB & 0x80FF; // RB8-RB14 as output
	TRISD = TRISD & 0xFF9F;	// RD5, RD6 as output
	
	LATDbits.LATD5 = 0;
	LATDbits.LATD6 = 1;

	while(1){
		c = getChar();
		LATB = (LATB && 0x80FF); // reset
		switch(c){
			case 'A':
			case 'a': 
				LATBbits.LATB8 = 1;break; // LATB = (LATB & 0x80FF) | 0x0100;
			case 'B':
			case 'b': 
				LATBbits.LATB9 = 1;break;
			case 'C':
			case 'c': 
				LATBbits.LATB10 = 1;break;
			case 'D':
			case 'd': 
				LATBbits.LATB11 = 1;break;
			case 'E':
			case 'e': 
				LATBbits.LATB12 = 1;break;
			case 'F':
			case 'f': 
				LATBbits.LATB13 = 1;break;
			case 'G':
			case 'g': 
				LATBbits.LATB14 = 1;break;
			default: 
				break;
		}
	}
	return 0;
}

void delay(unsigned int ms){
	resetCoreTimer();
	while(readCoreTimer() < K * ms);
}
