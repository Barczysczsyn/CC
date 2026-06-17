#leia a

addi $v0,$zero,5
syscall
add $s0,$zero,$v0


#if(a>9
slti $t0,$s0,10
bne $t0, $zero,ELSE



#escreva >0=9
addi $v0,$zero,11,
addi $a0,$zero,62
syscall

addi $a0,$zero,57
syscall


j FIM_ELSE
ELSE:


addi $v0,$zero,11
addi $a0,$zero,60
syscall
addi $a0,$zero,61
syscall
addi $a0,$zero,57
syscall
FIM_ELSE:
addi $v0,$zero,10