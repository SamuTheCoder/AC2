	.equ TRISB,0x6040
	.equ PORTB,0x6050
	.equ LATB,0x6060

	.equ TRISE,0x6100
	.equ PORTE,0x6110
	.equ LATE,0x6120	

	.equ SFR_BASE_HI,0xBF88
	.equ PRINT_INT10,7
	.equ READ_CORE_TIMER,11
	.equ RESET_CORE_TIMER,12
	.data
	.text
	.globl main

main: 	addiu $sp, $sp, -8
	sw $ra, 0($sp)
	sw $s0, 4($sp)
	
conf:	lui $t0, SFR_BASE_HI
	lw $t1, TRISE($t0)
	andi $t1, $t1, 0xFF83
	sw $t1, TRISE($t0)
	
	li $s0, 1 #counter = 0
	
while:	lui $t0, SFR_BASE_HI

	lw $t1, LATE($t0)
	andi $t1, 0xFF83
	sll $t2, $s0, 2
	or $t1, $t1, $t2 #colocar o valor certo no sitio
	sw $t1, LATE($t0)
	
	li $a0, 435
	jal delay
	
	sll $s0, $s0, 1
if:	ble $s0, 0x0010, endif
	li $s0, 1
endif:	
	j while
endw:	lw $ra, 0($sp)
	lw $s0, 4($sp)
	addiu $sp, $sp, 8

	li $v0, 0
	jr $ra


######################################## delay #####################################

	# Função terminal que gera um atraso temporal programável de 1 ms
	# void delay(unsigned int ms)

	.text
	.globl delay
delay:	
	li $v0,RESET_CORE_TIMER	
	syscall				# resetCoreTimer();
while2:
	li $v0,READ_CORE_TIMER
	syscall
	mul $t0,$a0,20000
	blt $v0,$t0,while2		# while(readCoreTimer() < k * ms);
	
	jr $ra				# Fim da função delay		
