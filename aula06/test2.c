#include <detpic32.h>

#define K (FREQ / 2)

unsigned char toBcd(unsigned char value);
void send2displays(unsigned char value);
void delay(unsigned int ms);

int main(){
	TRISD = TRISD & 0xFF9F;
	TRISB = TRISB & 0x80FF;
	TRISE = TRISE & 0xFFFD;

	// configurar porto RB4 como entrada analógica
	TRISBbits.TRISB4 = 1;	// RB4 configurado como saída digital
	AD1PCFGbits.PCFG4 = 0;	// RB4 configurado como analógico -> AN4
	AD1CON1bits.SSRC = 7;	// selecionar os bits, de forma a quando forem alterados, a conversão comece

	AD1CON1bits.CLRASAM = 1;	// Terminar a conversão quando é gerado a 1ª interrupção A/D

	AD1CON3bits.SAMC = 16;		// Tempo de sample é 16 TAB ou (TAB = 100ns)

	AD1CON2bits.SMPI = 1;	// apenas 5 conversões consecutiva
	AD1CHSbits.CH0SA = 4;	// selecionar AN4 como input do ADC

	AD1CON1bits.ON = 1;		// Ativar o conversor A/D
	
	LATEbits.LATE1 = 0;
	while(1){
		AD1CON1bits.ASAM = 1;	// Iniciar a conversão
		while( IFS1bits.AD1IF == 0 );
		int * p = (int *) (&ADC1BUF0);
		int sum = 0;
		int i;
		for (i = 0; i < 3; i++){
			sum += p[i * 4];
		}
		int average = sum / 2;
		printInt(average, 16 | 3 << 16);
		putChar('\r');
		int toDisplay = average / 110;
		send2displays(toDisplay);
		delay(200);
		
		IFS1bits.AD1IF == 0;
		LATEbits.LATE1 = 1  - LATEbits.LATE1;	
	}
}

unsigned char toBcd(unsigned char value){
	return ((value / 10) << 4) + (value % 10);
}

void send2displays(unsigned char value){
	static const int display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
	
	LATDbits.LATD5 = 1;
	LATDbits.LATD6 = 0;
	LATB = (LATB & 0x80FF) | (unsigned int) (display7Scodes[value & 0x00FF] << 8);
	
}

void delay(unsigned int ms){
	resetCoreTimer();
	while(readCoreTimer() < (K/1000) * ms);
}
