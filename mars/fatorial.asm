j COMECO

FATORIAL:

#se for 0 ou 1
slti $t1,$a0,2

beq $t1,$zero,FIM_IF
	addi $v0,$zero,1
	jr $ra
FIM_IF:

#caso recursivo

#salva o ra
addi $sp,$sp,-4
sw $ra,0($sp)
#salva o a0
addi $sp,$sp,-4
sw $a0,0($sp)

#coloca o argumento do proximo pra n-1
addi $a0,$a0,-1

jal FATORIAL

#pega o a0
lw $a0,0($sp)
addi $sp,$sp,4
#pega de volta o ra
lw $ra,0($sp)
addi $sp,$sp,4


mul $v0,$v0,$a0

#o retorno é em v0
#add $v0,$zero,$a0

jr $ra


COMECO:

addi $v0,$zero,5
syscall

add $a0,$zero,$v0

jal FATORIAL

add $a0,$v0,$zero

addi $v0,$zero,1
syscall