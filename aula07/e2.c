#include <detpic32.h>

#define K (PBCLK / 1000)

void delay(unsigned int ms);
void send2displays(unsigned char value);
int voltage_converter(int val_AD);
unsigned char toBcd(unsigned char value);

volatile unsigned char voltage = 0; // Global variable

//_int_ creates prolog and epilog
void _int_(27) isr_adc(void)
{
	// Read 8 samples (ADC1BUF0, ..., ADC1BUF7) and calculate average
	// Calculate voltage amplitude
	// Convert voltage amplitude to decimal and store the result in the
	//global variable "voltage"
	int *p = (int *) &ADC1BUF0;
    int i, average = 0;

    for (i = 0; i < 8; i++)
        average += p[i * 4];

    voltage = toBcd(voltage_converter(average / 8));
	// Reset AD1IF flag
	IFS1bits.AD1IF = 0;
}

int main(void)
{
	unsigned int cnt = 0;
	// Configure all (digital I/O, analog input, A/D module, interrupts)
	TRISBbits.TRISB4 = 1; //port RB4 as analogic entry
	TRISB = TRISB & 0x80FF; //ports RB14-RB8 as output
	TRISD = TRISD & 0xFF9F; //ports RD6-RD5 as output
	AD1PCFGbits.PCFG4 = 0;	// RB4 configured as analogic (AN4)
	AD1CON1bits.SSRC = 7;	

	AD1CON1bits.CLRASAM = 1;	// end convertion when the first A/D interruption occurs

	AD1CON3bits.SAMC = 16;		

	AD1CON2bits.SMPI = 8-1;	// 8 convertions
	AD1CHSbits.CH0SA = 4;	// select AN4 as input for ADC
	
	// Enable interrupts ADC
	IPC6bits.AD1IP = 2;         // configure priority of A/D interrupts
	IFS1bits.AD1IF = 0;         // clear A/D interrupt flag
	IEC1bits.AD1IE = 1;         // enable A/D interrupts

	AD1CON1bits.ON = 1;		// activate ADC converter

	EnableInterrupts(); // Global Interrupt Enable
	while(1)
	{
		if(cnt == 0) // 0, 200 ms, 400 ms, ... (5 samples/second, or 5HZ sample frequency)
		{
			AD1CON1bits.ASAM = 1;// Start A/D conversion
		}
		send2displays(voltage);// Send "voltage" value to displays
		cnt = (cnt + 1) % 20;
		// Wait 10 ms (100HZ refresh frequency)
		delay(10);
	}
	return 0;
}

void send2displays(unsigned char value){
	static const int display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
	static int displayFlag = 0;
	
	int digit_low = value & 0x0F;
	int digit_high = value >> 4;

	
	switch(displayFlag){
		case 0:
			LATDbits.LATD6 = 0;		// select display low -> RD5
			LATDbits.LATD5 = 1;
			LATB = (LATB & 0x80FF) | (unsigned int) (display7Scodes[digit_low] << 8); 
			break;
		case 1:
			LATDbits.LATD6 = 1;		// select display high -> RD6
			LATDbits.LATD5 = 0;
			LATB = (LATB & 0x80FF) | (unsigned int) (display7Scodes[digit_high] << 8); 
			break;
		default:
			break;
	}
	
	displayFlag = !displayFlag;
}


void delay(unsigned int ms){
	resetCoreTimer();
	while(readCoreTimer() < K * ms);
}

int voltage_converter(int val_AD){
	return (val_AD * 33 + 511) / 1023;
}

unsigned char toBcd(unsigned char value){
	return ((value / 10) << 4) + (value % 10);
}

