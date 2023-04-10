	.equ SFR_BASE_HI, 0xBF88
	.equ PORTD,0x60D0 	# Ler
	.equ TRISD,0x60C0 	# Selecionar Entrada/ Saída
	.equ TRISE, 0X6100 	# Selecionar Entrada/ Saída
	.equ LATE, 0X6120 	# Escrever
	
	.data
	.text
	.globl main
main:
	lui $t0, SFR_BASE_HI

	lw $t1, TRISD($t0) # address: BF800 + 00006040
	ori $t1, $t1, 0x0100 #RB0 as input 
	sw $t1, TRISD($t0) #RD8 configured as in

	lw $t1, TRISE($t0) #Read TRISE register
	andi $t1, $t1, 0xFFFE #RE0 as  output (1º bit - 0)
	sw $t1, TRISE($t0) #RE2 configured as out

while1:	lw $t1, PORTD($t0) #read PORTD register
	andi $t1, $t1, 0x0100 #reset all bits except bit 8
	srl $t1,$t1,8		# mover o bit 8 para a posição 0 (para ser isolado no andi abaixo)

	xori $t1, $t1, 0x0001	#negation of RD8
	lw $t2, LATE($t0) #read LATE register
	andi $t2, $t2, 0xFFFE #reset of RE0
	or $t2, $t2, $t1 #merge data

	sw $t2, LATE($t0) #write LATE register
	j while1

endwhile:
	jr $ra
