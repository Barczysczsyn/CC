.data
	.space 8
.text


addi $v0,$zero,5
syscall
add $s0,$zero,$v0


lui $s1,0x1001
ori $s1,$s1,0x0000


#var i
addi $s2,$zero,0
#for(i=0;i<n,ptr=ptr->prox) //i-> $s2
FOR_READ:
	slt $t0,$s2,$s0
	beq $t0,$zero,FIM_FOR_READ
	#leia c
	addi $v0,$zero,12
	syscall
	add $s3,$zero,$v0
	#armazene c no fim de L
	#alocar um no, cada no tem 8 bytes
	addi $v0,$zero,9
	addi $a0,$zero,8
	syscall
	#guardar o c neste no
	sb $s3,0($v0) #char c
	#c->prox = NULL
	sw $zero,4($v0) 
	#fazer ptr->prox = novo no
	sw $v0,4($s1)
	
	#++i,ptr=ptr->prox
	addi $s2,$s2,1
	lw $s1,4($s1)
	
	j FOR_READ
	
FIM_FOR_READ:

#imprima a lista
lui $s2,0x1001 #ptr=&no cabeça
lw $s2,4($s2) #ptr=ptr->prox

#for(ptr = endereco do no cabeca, ptr=ptr->prox ; ptr->prox != nuLl;ptr = ptr->prox)
FOR_PRINT:
	beq $s2,$zero,FIM_FOR_PRINT
	#imprima ptr->c
	addi $v0,$zero,11
	lb $a0,0($s1)
	syscall
	lw $s1,4($s1)
	j FOR_PRINT
	
FIM_FOR_PRINT: