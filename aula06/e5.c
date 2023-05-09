#include <detpic32.h>

#define K (PBCLK / 1000)

int voltage_converter(int val_AD);
void delay(unsigned int ms);

int main(void){

	TRISBbits.TRISB4 = 1;	// port RB4 as analogic entry
	AD1PCFGbits.PCFG4 = 0;	// RB4 configured as analogic (AN4)
	AD1CON1bits.SSRC = 7;	

	AD1CON1bits.CLRASAM = 1;	// end convertion when the first A/D interruption occurs

	AD1CON3bits.SAMC = 16;		

	AD1CON2bits.SMPI = 4-1;	// 4 convertions
	AD1CHSbits.CH0SA = 4;	// select AN4 as input for ADC

	AD1CON1bits.ON = 1;		// activate ADC converter

	int i, vol;
	while(1){
		AD1CON1bits.ASAM = 1;	// Start conversion
		while( IFS1bits.AD1IF == 0 );	// end for the end of the convertion (polling)

		int *p = (int *)(&ADC1BUF0);

		vol = 0;
		for( i = 0; i < 16; i++ ) {
			vol += voltage_converter(p[i*4]);
		}
		printInt(vol/4, 10 | 2 << 16); // average (0-33)
		putChar('\r');
		IFS1bits.AD1IF = 0;		// reset

	}

	return 0;
}

int voltage_converter(int val_AD){
	return (val_AD * 33 + 511) / 1023;
}

void delay(unsigned int ms){
	resetCoreTimer();
	while(readCoreTimer() < K * ms);
}
