
#insira a nota

addi $v0 ,$zero, 5
syscall
#inteiro fica em $v0

#guarda o v0 em $s0
add $s0, $v0, $zero
#escreve eu
addi $v0,$zero,11
addi $a0,$zero,101
syscall
addi $a0,$zero,117
syscall
#espaco
addi $a0,$zero,32
syscall



#pula se for maior
slti $t0,$s0,6
beq $t0,$zero,ELSE


#é menor

#addi $v0,$zero,11
addi $a0,$zero,79
syscall
addi $a0,$zero,68
syscall
addi $a0,$zero,69
syscall
addi $a0,$zero,73
syscall
addi $a0,$zero,79
syscall

#espaco
addi $a0,$zero,32
syscall




j FIM_ELSE
#e maior
ELSE:

addi $a0,$zero,97
syscall
addi $a0,$zero,109
syscall
addi $a0,$zero,111
syscall

#espaco
addi $a0,$zero,32
syscall

FIM_ELSE:


#o de

addi $a0,$zero,111
syscall

#espaco
addi $a0,$zero,32
syscall


addi $a0,$zero,68
syscall
addi $a0,$zero,101
syscall
addi $a0,$zero,108
syscall
addi $a0,$zero,97
syscall
addi $a0,$zero,105
syscall
addi $a0,$zero,114
syscall
