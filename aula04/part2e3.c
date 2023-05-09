#include <detpic32.h>

#define K (PBCLK / 1000)

void delay(unsigned int ms);

int main(void){
	
	unsigned int segment;
	int i;

	TRISB = TRISB & 0x80FF; // RB8-RB14 as output
	TRISD = TRISD & 0xFF9F;	// RD5, RD6 as output
	
	LATD = (LATD & 0xFF9F) | 0x0020;

	while(1){
		segment = 0x0100; // saber pq é que o professor usa um unsigned char e inicializa isto a 1
		
		for(i = 0; i < 7; i++){
			LATB = (LATB & 0x80FF) | segment; // enviar o valor do segment para o display
			delay(250);	// esperar 0.5s -> 500
			segment = segment << 1;
		}
		
		LATD = LATD ^ 0x0060; //XOR to switch the displays
		


	}
	return 0;
}

void delay(unsigned int ms){
	resetCoreTimer();
	while(readCoreTimer() < K * ms);
}
