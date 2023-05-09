#include <detpic32.h>

#define K (PBCLK / 1000)

void delay(unsigned int ms);
void send2displays(unsigned char value);
unsigned char toBcd(unsigned char value);

int main(void){
	TRISB = TRISB & 0x80FF; // RB8-RB14 as output
	TRISD = TRISD & 0xFF9F; // RD5-RD6 as output
	TRISE = TRISE & 0xFF00; // RE7-RE0 as output
	
	unsigned int i = 0;
	unsigned int counter = 0;
	while(1){
		i = 0;
		do{
			send2displays(toBcd(counter)); //write the number on the displays in decimal
			delay(10); //10ms = 100Hz
			LATE = (LATE & 0xFF00) | toBcd(counter); // 0xFF00 is used to clean the 8 LSB'S
		}while(++i < 50); //i goes from 1 to 10 to decrease the frequency from 100Hz to 2Hz
		if(++counter>=60) counter = 1;
	}
	return 0;
}

void send2displays(unsigned char value){
	static const int display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
	static int displayFlag = 0;
	
	int digit_low = value & 0x0F;
	int digit_high = value >> 4;

	
	switch(displayFlag){
		case 0:
			LATDbits.LATD6 = 0;		// select display low -> RD5
			LATDbits.LATD5 = 1;
			LATB = (LATB & 0x80FF) | (unsigned int) (display7Scodes[digit_low] << 8); 
			break;
		case 1:
			LATDbits.LATD6 = 1;		// select display high -> RD6
			LATDbits.LATD5 = 0;
			LATB = (LATB & 0x80FF) | (unsigned int) (display7Scodes[digit_high] << 8); 
			break;
		default:
			break;
	}
	
	displayFlag = !displayFlag;
}

unsigned char toBcd(unsigned char value)
{
	return ((value / 10) << 4) + (value % 10);
}

void delay(unsigned int ms){
	resetCoreTimer();
	while(readCoreTimer() < K * ms);

}
