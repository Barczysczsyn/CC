

addi $s0,$zero,0
addi $s1,$zero,0

FOR_TESTE:
	slti $t0,$s0,5
	#desvia quando t0 é igual a zero
	beq $t0,$zero,FIM_FOR
	
	#syscall 5 lê inteiro
	addi $v0,$zero,5
	syscall
	add $s1,$s1,$v0
	
	addi $s0,$s0,1
	j FOR_TESTE

FIM_FOR:

#imprimir soma
#syscall 1 imprime inteiro
addi $v0,$zero,1
syscall