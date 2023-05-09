#include <detpic32.h>

#define K (PBCLK / 1000)

int main(void){
	TRISBbits.TRISB4 = 1;	// port RB4 as analogic entry
	AD1PCFGbits.PCFG4 = 0;	// RB4 configurado como analógico -> AN4
	AD1CON1bits.SSRC = 7;	

	AD1CON1bits.CLRASAM = 1;	// end convertion when the first A/D interruption occurs

	AD1CON3bits.SAMC = 16;		

	AD1CON2bits.SMPI = 4-1;	// 4 convertions
	AD1CHSbits.CH0SA = 4;	// select AN4 as input for ADC

	AD1CON1bits.ON = 1;		// activate ADC converter

	int i;
	while(1){
		AD1CON1bits.ASAM = 1;	// Iniciar a conversão
		while( IFS1bits.AD1IF == 0 );	// Esperar pela conclusão da conversão -> polling
		int *p = (int *)(&ADC1BUF0);
		for( i = 0; i < 16; i++ ) {
			printInt( p[i*4], 16 | 3 << 16 );
			putChar(' ');
		}
		putChar('\r');
		IFS1bits.AD1IF = 0;		// desativar/reset manual da operação

	}

	return 0;
}
