#int main(void)
#{
#	int value;
#	while(1)
#	{
#		printStr("\nIntroduza um inteiro (sinal e módulo): ");
#		value = readInt10();
#		printStr("\nValor em base 10 (signed): ");
#		printInt10(value);
#		printStr("\nValor em base 2: ");
#		printInt(value, 2);
#		printStr("\nValor em base 16: ");
#		printInt(value, 16);
#		printStr("\nValor em base 10 (unsigned): ");
#		printInt(value, 10);
#		printStr("\nValor em base 10 (unsigned), formatado: ");
#		printInt(value, 10 | 5 << 16); // ver nota de rodapé 3
#	}
#	return 0;
#}
	
	.equ READ_INT10,5
	.equ PRINT_INT10,7
	.equ PRINT_INT,6 # $a0 = value, $a1 = base
	.equ PRINT_STRING,8
	.data
str1:	.asciiz "\nIntroduza um inteiro (sinal e módulo): "
str2:	.asciiz "\nValor em base 10 (signed): "
str3: 	.asciiz "\nValor em base 2: "
str4: 	.asciiz "\nValor em base 16: "
str5:	.asciiz "\nValor em base 10 (unsigned): "
str6:	.asciiz "\nValor em base 10 (unsigned), formatado: "
	.text
	.globl main
main:
while:	la $a0, str1
	li $v0, PRINT_STRING
	syscall
	li $v0, READ_INT10
	syscall
	move $t0, $v0
	
	la $a0, str2
	li $v0, PRINT_STRING
	syscall
	move $a0, $t0
	li $v0, PRINT_INT10
	syscall
	
	la $a0, str3
	li $v0, PRINT_STRING
	syscall
	move $a0, $t0
	li $a1, 2
	li $v0, PRINT_INT
	syscall
	
	la $a0, str4
	li $v0, PRINT_STRING
	syscall
	move $a0, $t0
	li $a1, 16
	li $v0, PRINT_INT
	syscall
	
	la $a0, str5
	li $v0, PRINT_STRING
	syscall
	move $a0, $t0
	li $a1, 10
	li $v0, PRINT_INT
	syscall
	
	la $a0, str6
	li $v0, PRINT_STRING
	syscall	
	move $a0, $t0
	li $a1,0x0005000A
	li $v0, PRINT_INT
	syscall
	
	j while
endwhile:
	li $v0, 0
	jr $ra
