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
main: 	li $t0,0 # counter=0
while: 	# while (1) {
	li $v0,RESET_CORE_TIMER #
	syscall # resetCoreTimer()
	
while2:	li $v0, READ_CORE_TIMER
	syscall
	blt $v0, 200000, while2
	
endw2:	li $t1, 4
	sll $t1, $t1, 16
	li $t2, 10
	or $t2, $t2, $t1
	
	li $v0, PRINT_INT
	move $a0, $t0
	move $a1, $t2
	syscall
	
	addi $t0, $t0, 1

	li $v0, PUT_CHAR
	li $a0, '\r'
	syscall
	
	j while
endw:	
	jr $ra #
