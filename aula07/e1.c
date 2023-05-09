// Com o _int_ o compilador automaticamente cria o prólogo e o epílogo 
#include <detpic32.h>

#define K (PBCLK / 1000)

// Interrupt service routine (interrupt handler)
void _int_(27) isr_adc(void) // Replace VECTOR by the A/D vector
			    // number - see "PIC32 family data
			   // sheet" (pages 74-76)
{
	// Read conversion result (ADC1BUF0) and print it
	int value = *(int*)(&ADC1BUF0);

	printInt(value, 10 | 4 << 16);
	putChar('\r');

	AD1CON1bits.ASAM = 1;	// Start A/D conversion

	IFS1bits.AD1IF = 0;		// Reset AD1IF flag
}

int main(void)
{
	// Configure all (digital I/O, analog input, A/D module)
	TRISBbits.TRISB4 = 1;	// port RB4 as analogic entry
	AD1PCFGbits.PCFG4 = 0;	// RB4 configurado como analógico -> AN4
	AD1CON1bits.SSRC = 7;	

	AD1CON1bits.CLRASAM = 1;	// end convertion when the first A/D interruption occurs

	AD1CON3bits.SAMC = 16;		

	AD1CON2bits.SMPI = 4-1;	// 4 convertions
	AD1CHSbits.CH0SA = 4;	// select AN4 as input for ADC

	AD1CON1bits.ON = 1;		// activate ADC converter

	// Configure interrupt system
	IPC6bits.AD1IP = 2;		// priority
	IFS1bits.AD1IF = 0;		// clear A/D interrupt flag
	IEC1bits.AD1IE = 1;		// enable A/D interrupts

	EnableInterrupts();		// Global Interrupt Enable
	
	AD1CON1bits.ASAM = 1;	// Iniciar a conversão
	while(1); // all activity is done by the ISR
	return 0;
}
