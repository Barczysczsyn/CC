
#ler n
addi $v0,$zero,5
syscall

#s0 e n
add $s0,$zero,$v0

#aloca n bits
add $a0,$zero,$s0
syscall

#s1 é V
add $s1,$zero,$v0


#inicializa k
#$t1 é k
addi $t1,$zero,0

#ler todos os numero de V
#cria V temporario
add $t4, $s1,$zero
FOR_V:
	slt $t0,$t1,$s0
	beq $t0,$zero,FIM_FOR_V
	
	#ler o V[k]
	addi $v0,$zero,5
	syscall
	
	#coloca o int no v[4*k]
	sb $v0,4($t4)
	
	#k++
	addi $t1,$t1,1


	j FOR_V
FIM_FOR_V:





#inicializa j
#$t2 é j
addi $t2,$zero,0

#for externo
FOR_EXTERNO:
	slt $t0,$t2,$s0
	beq $t0,$zero,FIM_FOR_EXTERNO
	
	
	#inicializa i
	#$t3 é i
	addi $t3,$zero,0
	
	
	#V[i] e $s2
	add $s2, $zero,$s1
	
	
	#for externo
	FOR_INTERNO:
		slt $t0,$t3,$s0
		beq $t0,$zero,FIM_FOR_INTERNO
		
		
		#IF
		
		#t5 e v{i]
		lb $t5,4($s2)
		
		
		#t6 é V[i+1]
		addi $t7,$s2,1
		lb $t6,4($t7)
		
		#FIXME ele tambem vai trocar se for igual, oq é desnecessario
		slt $t0,$t5,$t6
		beq $t0,$zero,FIM_IF
		
		#entrou no if
		add $t8,$t5,$zero
		sw $t6,4($s2)
		sw $t8,4($t7)
		
		FIM_IF:
		
		#i++
		addi $t3,$t3,1
		
		#V[i]++
		#TODO 1 ou 4?
		addi $s2,$s2,1
		j FOR_INTERNO
	FIM_FOR_INTERNO:
	
	#j++
	addi $t2,$t2,1
	
	j FOR_EXTERNO
FIM_FOR_EXTERNO:
