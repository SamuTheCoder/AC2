#include <detpic32.h>

#define K (PBCLK / 1000) // constante K dinâmica -> PBCLK é metade do FREQ

void delay(unsigned int ms); // declare the function to the compiler

int main(void)
{
	// Configure port RC14 as output
	TRISCbits.TRISC14 = 0;
	while(1)
	{
		// Wait 0.5s
		delay(500);
		// Toggle RC14 port value
		LATCbits.LATC14 = !LATCbits.LATC14;
	}
	return 0;
}

void delay(unsigned int ms){
	resetCoreTimer();
	// valor default de K é 20000
	while(readCoreTimer() < K * ms);
}
