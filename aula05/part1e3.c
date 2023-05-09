#include <detpic32.h>

#define K (PBCLK / 1000)

void delay(unsigned int ms);
void send2displays(unsigned char value);

int main(void){
	TRISB = TRISB & 0x80FF; // RB8-RB14 as output
	TRISD = TRISD & 0xFF9F; // RD5-RD6 as output

	while(1){
		send2displays(0x15); // Write 0x15 (decimal 15 on the displays)
		delay(10); // 0,2s = 5 Hz = 4 000 000; ms = 200;
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

void delay(unsigned int ms){
	resetCoreTimer();
	while(readCoreTimer() < K * ms);

}
