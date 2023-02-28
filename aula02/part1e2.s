#1ms = 1000hz, 1s -> 20000000, 1ms -> x, x = 0.001*20000000/1 = 20000
#answer is 20000
# Para o valor da frequência, alterar a constante do while para:
# 10 Hz -> 2.000.000
# 5 Hz -> 4.000.000
# 1 Hz -> 20.000.000
# 20000000 -> 1s(1hz), x -> ttarget, fazer as contas para as freqs
	.equ READ_CORE_TIMER,11
	.equ RESET_CORE_TIMER,12
	.equ PUT_CHAR,3
	.equ PRINT_INT,6
	.data
	.text
	.globl main
main:	addi $sp, $sp, -8
	sw $ra, 0($sp)
	sw $s0, 4($sp)

	li $s0,0 # counter=0
while: 	# while (1) {
	li $a0, 1 #delay(1ms)
	jal delay

	li $v0, PRINT_INT
	move $a0, $s0
	li $a1, 0x0004000A
	syscall
	
	addi $s0, $s0, 1 #se fosse ++counter era antes, counter++ é depois

	li $v0, PUT_CHAR
	li $a0, '\r'
	syscall
	
	j while
endw:	
	lw $s0, 4($sp)
	lw $ra, 0($sp)
	addi $sp, $sp, 8
	jr $ra 

##################################################
#delay
##################################################

	.text
	.globl delay
delay:
	li $v0, RESET_CORE_TIMER
	syscall
	mul $t0, $a0, 20000
while3:
	li $v0, READ_CORE_TIMER
	syscall
	blt $v0, $t0, while3
endw3:
	jr $ra
