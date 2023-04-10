#int main(){
#	cnt1 = 0; works at 1HZ (one increment each 10 times)
# 	cnt5 = 5; works at 5HZ (one increment each 2 times)
# 	cnt10 = 10; works at 10HZ (one increment each time)
#	time = 100;
#	while(1){
#		mode_in = inkey()
#		if (mode_in == N)
#			time = 200
#		else if (mode_in == A)
#			time = 50;
#		delay(time)
#		cnt10++;
#		if(cnt10 % 2 == 0)
#			cnt5++;
#		if(cnt10 % 10 == 0)
#			cnt1++;
#		printInt(cnt1, 10 | 4 << 16);
#		putChar('\t'); // tab
#		printInt(cnt5, 10 | 4 << 16);
#		putChar('\t');
#		printInt(cnt10, 10 | 4 << 16);
#		putChar('\r');
#	}
#}
	.equ READ_CORE_TIMER,11
	.equ RESET_CORE_TIMER,12
	.equ PUT_CHAR,3
	.equ PRINT_INT, 6
	.equ INKEY, 1
	.text
	.globl main
main:	
	addi $sp, $sp, -20 #salvaguarda de registos
	sw $ra, 0($sp)
	sw $s0, 4($sp)
	sw $s1, 8($sp)
	sw $s2, 12($sp)
	sw $s3, 16($sp)

	li $s0, 0 #cnt1
	li $s1, 0 #cnt5
	li $s2, 0 #cnt10
	li $s3, 100 #
	
while:	
	li $v0, INKEY
	syscall
	
ifmod:	li $t0, 'N'
	bne $v0, $t0, elmode
	li $s3, 200
	
elmode:	li $t0, 'A'
	bne $v0, $t0, endmod
	li $s3, 50
	
endmod:	move $a0, $s3
	jal delay
	
	addi $s2, $s2, 1 #cnt10++
if1:	
#	li $t1, 2
#	div $s2, $t1
#	mfhi $t2
	rem	$t2, $s2, 2
	bne $t2, $0, if2 # if (cnt10 % 2 == 0)

	addi $s1, $s1, 1 #cnt5++

if2:    
#	li $t3, 10
#	div $s2, $t3
#	mfhi $t4
	rem $t4, $s2, 10
	bne $t4, $0, endif # if (cnt10 % 10 == 0)

	addi $s0, $s0, 1 # cnt1++

endif:	
	li $v0, PRINT_INT
	move $a0, $s0
	li $a1, 0x0005000A
	syscall		   #printInt(cnt1, 10 | 4 << 16)

	li $v0, PUT_CHAR
	li $a0, '\t'
	syscall		   # tab
	
	li $v0, PRINT_INT
	move $a0, $s1
	li $a1, 0x0005000A
	syscall            #printInt(cnt5, 10 | 4 << 16)

	li $v0, PUT_CHAR
	li $a0, '\t'
	syscall            # tab
	
	li $v0, PRINT_INT
	move $a0, $s2
	li $a1, 0x0005000A
	syscall            #printInt(cnt10, 10 | 4 << 16)

	li $v0, PUT_CHAR
	li $a0, '\r'
	syscall
	
	j while
	
endw:	
	lw $s3, 16($sp)
	lw $s2, 12($sp)
	lw $s1, 8($sp)
	lw $s0, 4($sp)
	lw $ra, 0($sp) 	
	addi $sp, $sp, 20
	
	li $v0, 0
	jr $ra
	
######################################## 
#delay 
########################################

	# Função terminal que gera um atraso temporal programável de x ms
	# void delay(unsigned int ms)

#	.text
#	.globl delay
delay:
	li $v0, RESET_CORE_TIMER
	syscall
	mul $t0, $a0, 20000
while2:
	li $v0, READ_CORE_TIMER
	syscall
	blt $v0, $t0, while2
endw2:
	jr $ra

