#include <detpic32.h>

#define K (PBCLK / 1000) // constante K dinâmica -> PBCLK é metade do FREQ

void delay(unsigned int ms); // declare the function to the compiler

int main(void)
{
	// Configure port RE6-RE3 as outputs
	TRISE = TRISE & 0xFF87;
	
	int counter = 0x0;
	while(1)
	{
		// Wait 0.25s
		delay(250);
		LATE = (LATE & 0xFF87) | counter;
		
		counter += (1 << 3);
		//preserve the state of the R6-R3 of the counter variable and ignore the remaining bits
		counter = counter & 0x0078;
	}
	return 0;
}

void delay(unsigned int ms){
	resetCoreTimer();
	// valor default de K é 20000
	while(readCoreTimer() < K * ms);
}
