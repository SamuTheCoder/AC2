#int main(void)
#{
#		char c;
#	do {
#		c = inkey();
#	if( c != 0 )
#		putChar( c );
#	else
#		putChar('.');
#	} while( c != '\n' );
#	return 0;
#}

	.equ PUT_CHAR,3
	.equ INKEY, 1
	.text
	.globl main
main:
do:	li $v0, INKEY
	syscall
	move $t0, $v0
	li $v0, PUT_CHAR

if:	beqz $t0, else
	move $a0, $t0
	syscall

else:	li $a0, '.'
	syscall

while: 	bne $t0, '\n', do
	
	li $v0, 0
	jr $ra
