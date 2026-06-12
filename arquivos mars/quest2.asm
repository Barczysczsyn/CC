addi $v0,$zero,5
syscall
add $s0,$zero,$v0

# a>33
# a<34 pula

addi $s1,$zero,34

slt $t0,$s0,$s1
bne $t0,$zero,FIM_IF


addi $a0,$zero,1
addi $v0,$zero,1
syscall


FIM_IF: