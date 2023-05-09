#include <detpic32.h>

#define K (PBCLK / 1000)

int voltage_converter(int val_AD);
void delay(unsigned int ms);
void send2displays(unsigned char value);
unsigned char toBcd(unsigned char value);

int main(void){
	TRISB = TRISB & 0x80FF;
	TRISD = TRISD & 0xFF9F;
	
	TRISBbits.TRISB4 = 1;
	AD1PCFGbits.PCFG4 = 0;
	AD1CON1bits.SSRC = 7;	// selecionar os bits, de forma a quando forem alterados, a conversão comece

	AD1CON1bits.CLRASAM = 1;	// Terminar a conversão quando é gerado a 1ª interrupção A/D

	AD1CON3bits.SAMC = 16;		// Tempo de sample é 16 TAB ou (TAB = 100ns)
	
	AD1CON2bits.SMPI = 4; // 5 conversions (5-1)
	AD1CHSbits.CH0SA = 4; //AN4 as ADC input
	
	AD1CON1bits.ON = 1; 
	
	int i, vol = 0, cnt = 0;
	while(1){
		if(cnt++ % 50 == 0){ // 0, 200ms, 400ms, 600ms, ...
			AD1CON1bits.ASAM = 1;
			while(IFS1bits.AD1IF == 0);
			int *p = (int *) (&ADC1BUF0);
			vol = 0;
			for(i = 0; i< 5; i++){
				vol += voltage_converter(p[i*4]);
			}
		}
		send2displays(toBcd(vol/5));
		delay(10);
	}
}

int voltage_converter(int val_AD){
	return (val_AD * 33 + 511) / 1023;
}

unsigned char toBcd(unsigned char value){
	return ((value / 10) << 4) + (value % 10);
}

void send2displays(unsigned char value){
	static const int display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
	static char displayFlag = 0; // static variable: doesn't loose its
								// value between calls to function
	if(displayFlag){ // send value to display high 
		LATDbits.LATD6 = 1;		// select display high -> RD6
		LATDbits.LATD5 = 0;
		LATB = (LATB & 0x80FF) | (unsigned int) (display7Scodes[value >> 4] << 8); // index = value >> 4; 
								// << 8 é usado para colocar o valor nos bits correspondentes a RB14 - RB8
								// send digit_high (dh) to display:		dh = value >> 4
	}else{
		LATDbits.LATD6 = 0;		// select display low -> RD5
		LATDbits.LATD5 = 1;
		LATB = (LATB & 0x80FF) | (unsigned int) (display7Scodes[value & 0x0F] << 8); // index = value & 0x0F; 								// << 8 é usado para colocar o valor nos bits correspondentes a RB14 - RB8
	} 
	displayFlag = !displayFlag;	// alternar a variável displayFlag
}
		
void delay(unsigned int ms){
	resetCoreTimer();
	while(readCoreTimer() < K * ms);
}
