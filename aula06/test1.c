#include <detpic32.h>

#define K (PBCLK / 1000)

void delay(unsigned int ms);

int main(){
	TRISE = TRISE & 0xFFC3; //RE5-RE2 as output
	TRISB = TRISB | 0x0004; //RB2 as input
	unsigned int tOF = (1/2.3) * 1000;
	unsigned int tON = (1/5.2) * 1000;
	
	unsigned int cnt = 0;
	while(1){
		int freqVar = (PORTB & 0x0004) | 0x0000;
		LATE = (LATE & 0xFFC3) | (cnt << 2);
		
		if(freqVar) delay(tON);
		else delay(tOF);
		
		cnt += 1;
		if(cnt > 9) cnt = 0;
	}
	return 0;

}


void delay(unsigned int ms){
	resetCoreTimer();
	while(readCoreTimer() < K*ms);
}
