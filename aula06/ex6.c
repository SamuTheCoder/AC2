#include <detpic32.h>

#define K (PBCLK / 1000)

int voltage_converter(int val_AD);
void delay(unsigned int ms);
void send2displays(unsigned char value);
unsigned char toBcd(unsigned char value);

int main(void){
	TRISB = TRISB & 0x80FF;         // RB14 to RB8 as output
    TRISD = TRISD & 0xFF9F;         // Displays high/low as output

	TRISBbits.TRISB4 = 1;	// port RB4 as analogic entry
	AD1PCFGbits.PCFG4 = 0;	// RB4 configured as analogic (AN4)
	AD1CON1bits.SSRC = 7;	

	AD1CON1bits.CLRASAM = 1;	// end convertion when the first A/D interruption occurs

	AD1CON3bits.SAMC = 16;		

	AD1CON2bits.SMPI = 4-1;	// 4 convertions
	AD1CHSbits.CH0SA = 4;	// select AN4 as input for ADC

	AD1CON1bits.ON = 1;		// activate ADC converter

	int i, vol = 0, cnt = 0;
	while(1){
		if(cnt++ % 20 == 0){ // 0, 200ms, 400ms, 600ms, ...
			AD1CON1bits.ASAM = 1;	// start convertion
			while(IFS1bits.AD1IF == 0);	// polling
			
			vol = 0;
			int *p = (int *)(&ADC1BUF0);
			for( i = 0; i < 5; i++ ) { // sum
				vol += voltage_converter(p[i*4]);
			}
		}
		send2displays(toBcd(vol/5)); // Ecras -> 00 - 41
		delay(10);
		cnt = (cnt + 1) % 20;
		IFS1bits.AD1IF = 0;		// Reset AD1IF flag
	}

	return 0;
}

int voltage_converter(int val_AD){
	return (val_AD * 33 + 511) / 1023;
}

unsigned char toBcd(unsigned char value){
	return ((value / 10) << 4) + (value % 10);
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
