	.equ SFR_BASE_HI, 0xBF88
	.equ PORTB, 0x6050 	# Ler
	.equ TRISB, 0x6040 	# Selecionar Entrada/ Saída
	.equ TRISE, 0X6100 	# Selecionar Entrada/ Saída
	.equ LATE, 0X6120 	# Escrever
	
	.equ READ_CORE_TIMER, 11
	.equ RESET_CORE_TIMER, 12
	.equ PRINT_INT10, 7
	
	.data
	.text
	.globl main
main:	addiu $sp, $sp, -12
	sw $ra, 0($sp)
	sw $s0, 4($sp)
	sw $s1, 8($sp)
	
	lui $s1, SFR_BASE_HI		# $t0 = 0xBF880000
	
	li $s0, 0 #initiate counter
	
	lw $t0, TRISB($s1)
	ori $t0, $t0, 0x0002 #RB1 as input
	sw $t0, TRISB($s1)
	
	lw $t1, TRISE($s1) #Read TRISE register
	andi $t1, $t1, 0xFFF0 #RE4-R1 as output
	sw $t1, TRISE($s1) #RE4-RE1 configured as out
	
while1:	
	lw $t2, LATE($s1) #read LATE register
	andi $t2, $t2, 0xFFF0 #reset of RE4-RE1
	or $t2, $t2, $s0 #merge data

	sw $t2, LATE($s1) #write LATE register
	
	# DEBUG
	move $a0,$s0
	li $v0, PRINT_INT10
	syscall				# printInt10(counter);
	
	li $a0, 333
	jal delay
	
if0:	bne $s0, $0, if15
	ori $s0, $s0, 8
	j endif
	
if15:	beq $s0, 15, elsif
	bge $s0, 8, else
elsif:	srl $s0, $s0, 1
	j endif

else:	sra $s0, $s0, 1

endif:	andi $s0,$s0,0x000F 		# Isolar 4 bits (ou seja, impede a escrita nos registos 
					# acima de RB3 e impede o contador de ultrapassar 0xF)

	j while1

endwhile:
	lw $s1, 8($sp)
	lw $s0, 4($sp)
	lw $ra, 0($sp)
	addiu $sp, $sp, 12
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

