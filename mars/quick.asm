addi $v0,$zero,5

syscall
#s0 é n
add $s0,$zero,$v0

sll $a0,$s0,2
addi $v0,$zero,9
syscall

#s1 é V
add $s1,$zero,$v0

#s2 é o ponteiro do V
add $s2,$zero,$s1

add $t0,$zero,$s0
FOR_LER:
	beq $t0,$zero,FIM_FOR_LER
	
	addi $v0,$zero,5
	syscall
	
	sw $v0,0($s2)
	addi $s2,$s2,4
	
	
	addi $t0,$t0,-1
	j FOR_LER
	
FIM_FOR_LER:

add $a0,$s1,$zero
add $a1,$zero,$zero
addi $a2,$s0,-1

jal QUICK_SORT


add $t0,$s0,$zero


#reinicializa o ponteiro de V
add $s2,$zero,$s1


FOR_PRINT:
beq $t0,$zero,FIM_FOR_PRINT
	
	lw $a0, 0($s2)
	addi $s2,$s2,4
	addi $v0,$zero, 1
	syscall
	
	addi $t0,$t0,-1
	
	j FOR_PRINT
FIM_FOR_PRINT:


#encerrar
addi $v0,$zero,10
syscall
################################################################		FUNCAO QUICK SORT 		################################################################################

QUICK_SORT:
#i = left
add $t0,$zero,$a1

#j = right
add $t1,$zero,$a2

#reinicializa o ponteiro de V
add $s2,$zero,$a0


#left + right
add $t3,$t1,$t0

# / 2
srl $t3,$t3,1

# * 4
sll $t3,$t3,2

add $s2,$t3 ,$a0


# x é s3
lw $s3 ,0($s2)

#reinicializa o ponteiro de V
add $s2,$zero,$a0

WHILE_1:

beq $t0,$t1, FIM_WHILE_1
slt $t4,$t0,$t1
beq $t4,$zero,FIM_WHILE_1
	WHILE_2:
	#a[i] é s4
	sll $t3,$t0,2
	add $s2,$t3,$s2
	lw $s4,0($s2)
	
	
	#reinicializa o ponteiro de V
	add $s2,$zero,$a0
	
	#IF a[i] < x
	slt $t4, $s4,$s3
	beq $t4,$zero,FIM_WHILE_2
	
	# && i < right
	slt $t4 ,$t0,$a2
	beq $t4,$zero,FIM_WHILE_2
		
		addi $t0,$t0,1
	j WHILE_2
	FIM_WHILE_2:
	
	
	WHILE_3:

	#a[j] é s5
	sll $t3,$t1,2
	add $s2,$t3,$s2
	lw $s5,0($s2)
	
	#reinicializa o ponteiro de V
	add $s2,$zero,$a0
	
	#TODO ver se o maior que esta certo
	#IF a[j] > x
	slt $t4, $s3, $s5
	beq $t4,$zero,FIM_WHILE_3
	
	# && j > left
	slt $t4 ,$a1,$t1
	beq $t4,$zero,FIM_WHILE_3
		
		addi $t1,$t1,-1
		
	j WHILE_3
	FIM_WHILE_3:
	
	beq $t0,$t1,FIM_IF_1
	slt $t4, $t0,$t1
	beq $t4,$zero,FIM_IF_1
		
		#a[i] é s4
		sll $t3,$t0,2
		add $s2, $s2, $t3
		lw $s4,0($s2)
		
		#reinicializa o ponteiro de V
		add $s2,$zero,$a0
		
		#a[j] é s5
		sll $t3,$t1,2
		add $s2,$s2,$t3
		lw $s5,0($s2)
		
		#reinicializa o ponteiro de V
		add $s2,$zero,$a0

		
		#guardar os valores trocados
		sll $t3,$t0,2
		add $s2, $s2, $t3
		sw $s5,0($s2)
		
		#reinicializa o ponteiro de V
		add $s2,$zero,$a0
		
		#guardar os valores trocados
		#a[j] é s5
		sll $t3,$t1,2
		add $s2,$s2,$t3
		sw $s4,0($s2)
		
		
		#i++
		addi $t0,$t0,1
		
		#j--
		addi $t1,$t1,-1
		
		
	FIM_IF_1:
	j WHILE_1
FIM_WHILE_1:
	
	slt $t4,$a1,$t1
	beq $t4,$zero,FIM_IF_2
	
		#salva o ra
		addi $sp ,$sp,-4
		sw $ra, 0($sp)
		#salva o left
		addi $sp ,$sp,-4
		sw $a1, 0($sp)
		#salva o rights
		addi $sp ,$sp,-4
		sw $a2, 0($sp)
		#salva o i
		addi $sp ,$sp,-4
		sw $t0, 0($sp)
		#salva o j
		addi $sp ,$sp,-4
		sw $t1, 0($sp)
		
		#manda o V so pra garantir
		add $a0,$s1,$zero
		#left
		add $a1,$a1,$zero
		#j
		add $a2,$t1,$zero
		jal QUICK_SORT
		
		#pega o j
		addi $sp ,$sp,4
		lw $t1, 0($sp)
		#pega o i
		addi $sp ,$sp,4
		lw $t0, 0($sp)
		#pega o right
		addi $sp ,$sp,4
		lw $a2, 0($sp)
		#pega o left
		addi $sp ,$sp,4
		lw $a1, 0($sp)
		#pega o ra
		addi $sp ,$sp,4
		lw $ra, 0($sp)
		
	FIM_IF_2:
	
	slt $t4,$t0,$a2
	beq $t4,$zero,FIM_IF_3
	
	
		#salva o ra
		addi $sp ,$sp,-4
		sw $ra, 0($sp)
		#salva o left
		addi $sp ,$sp,-4
		sw $a1, 0($sp)
		#salva o rights
		addi $sp ,$sp,-4
		sw $a2, 0($sp)
		#salva o i
		addi $sp ,$sp,-4
		sw $t0, 0($sp)
		#salva o j
		addi $sp ,$sp,-4
		sw $t1, 0($sp)
		
		#manda o V so pra garantir
		add $a0,$s1,$zero
		#i
		add $a1,$t0,$zero
		#right
		add $a2,$a2,$zero
		jal QUICK_SORT
		
		#pega o j
		addi $sp ,$sp,4
		lw $t1, 0($sp)
		#pega o i
		addi $sp ,$sp,4
		lw $t0, 0($sp)
		#pega o right
		addi $sp ,$sp,4
		lw $a2, 0($sp)
		#pega o left
		addi $sp ,$sp,4
		lw $a1, 0($sp)
		#pega o ra
		addi $sp ,$sp,4
		lw $ra, 0($sp)
		
	FIM_IF_3:

jr $ra

