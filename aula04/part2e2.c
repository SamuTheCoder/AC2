#include <detpic32.h>

#define K (PBCLK / 1000)

void delay(unsigned int ms);

int main(void){
	
	unsigned int segment;
	int i;

	TRISB = TRISB & 0x80FF; // RB8-RB14 as output
	TRISD = TRISD & 0xFF9F;	// RD5, RD6 as output
	
	LATDbits.LATD5 = 1;
	LATDbits.LATD6 = 0;

	while(1){
		segment = 0x0100; // saber pq é que o professor usa um unsigned char e inicializa isto a 1
		
		for(i = 0; i < 7; i++){
			LATB = (LATB & 0x80FF) | segment; // segment to display
			delay(250);	// esperar 0.5s -> 500
			segment = segment << 1; //next segment selected
		}
		
		// Alternar a seleção do ecrã
		LATDbits.LATD5 = !LATDbits.LATD5;
		LATDbits.LATD6 = !LATDbits.LATD6;
		// Alternar a seleção do ecrã, usando xor
		


	}
	return 0;
}

void delay(unsigned int ms){
	resetCoreTimer();
	while(readCoreTimer() < K * ms);
}
