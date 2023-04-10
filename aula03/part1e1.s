	.equ SFR_BASE_HI, 0xBF88
	.equ PORTB, 0x6050 	# Ler
	.equ TRISB, 0x6040 	# Selecionar Entrada/ Saída
	.equ TRISE, 0X6100 	# Selecionar Entrada/ Saída
	.equ LATE, 0X6120 	# Escrever
	
	.data
	.text
	.globl main
main:
	lui $t0, SFR_BASE_HI

	lw $t1, TRISB($t0) # address: BF800 + 00006040
	ori $t1, $t1, 0x0001 #RB0 as input (1º bit - 1)
	sw $t1, TRISB($t0) #RB0 configured as in

	lw $t1, TRISE($t0) #Read TRISE register
	andi $t1, $t1, 0xFFFE #RE0 as  output (1º bit - 0)
	sw $t1, TRISE($t0) #RE2 configured as out

while1:	lw $t1, PORTB($t0) #read PORTB register
	andi $t1, $t1, 0x0001 #reset all bits except bit 0

	lw $t2, LATE($t0) #read LATE register
	andi $t2, $t2, 0xFFFE #reset of RE0
	or $t2, $t2, $t1 #merge data

	sw $t2, LATE($t0) #write LATE register
	j while1

endwhile:
	jr $ra
