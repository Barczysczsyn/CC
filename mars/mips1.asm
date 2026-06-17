#int_32 a;
#leia a
addi $v0,$zero,5
syscall
add $s0,$zero,$v0

#if(a>0)
#condicao

blez $s0,FIM_IF
#escreva ">0" // 62 48
	addi $v0,$zero,11
	addi $a0,$zero,62
	syscall
	addi $a0,$zero,48
	syscall
FIM_IF:




addi $v0,$zero,5
syscall
add $s0,$zero,$v0

#if(a>0)
#condicao

slti $t0, $s0,9
beq $t0,$zero,FIM_IF2
#escreva ">0" // 62 48
	addi $v0,$zero,11
	addi $a0,$zero,60
	syscall
	addi $a0,$zero,57
	syscall
FIM_IF2:
addi $v0,$zero,10




