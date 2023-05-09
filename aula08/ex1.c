#include <detpic32.h>

int main(void){
	
	// Configure Timer T3 (2 Hz with interrupts disabled)
	T3CONbits.TCKPS = 7; // 1:32 prescaler (i.e. fout_presc = 39 KHz)
	PR3 = 39062; // Fout = 20MHz / (256 * (39062 + 1)) = 2 Hz
	TMR3 = 0; // Clear timer T2 count register
	T3CONbits.TON = 1; // Enable timer T2 (must be the last command of the
	// timer configuration sequence)
	IFS0bits.T3IF = 0; // Reset timer T2 interrupt flag
	while(1){
		while((IFS0bits.T3IF == 0));
		putChar('.');
		IFS0bits.T3IF = 0; // Reset timer T2 interrupt flag
	}
	return 0;
}
