#ler a
addi $v0,$zero,5
syscall
#$s0 é a
add $s0, $zero,$v0

#ler b
addi $v0,$zero,5
syscall
#$s1 é b
add $s1, $zero,$v0

slt $t0,$s1,$s0
beq $t0,$zero,ELSE

add $a0,$s0,$zero
addi $v0,$zero,1
syscall
j FIM_ELSE

ELSE:

add $a0,$s1,$zero
addi $v0,$zero,1
syscall

FIM_ELSE: