#ler n
addi $v0,$zero,5
syscall

#s0 e n
add $s0,$zero,$v0

#t5 é simplesmente 4
addi $t5,$zero,4
#n=n*4
mul $s3,$s0,$t5
#aloca n*4 bits
addi $v0,$zero,9
add $a0,$zero,$s3
syscall

#s1 é V
add $s1,$zero,$v0


#inicializa k
#$t1 é k
add $t1,$zero,$zero

#ler todos os numero de V
#cria V temporario
add $t4, $s1,$zero
FOR_V:
	slt $t0,$t1,$s0
	beq $t0,$zero,FIM_FOR_V
	
	#ler o V[k]
	addi $v0,$zero,5
	syscall
	
	#coloca o int no v[k]
	sw $v0,0($t4)
	
	#k++
	addi $t1,$t1,1
	
	#v[k++]
	#sw $t4,4($t4)
	addi $t4, $t4,4
	##é assim? FIXME


	j FOR_V
FIM_FOR_V:


#cria V temporario
add $t4, $s1,$zero

#t1 é i
add $t1,$zero,$zero
FOR_PRINT:
	slt $t0,$t1,$s0
	beq $t0,$zero,FIM_FOR_PRINT
	
	#coloca o numero em a0 pra printar
	lw $a0 ,0($t4)
	#printa
	addi $v0,$zero,1
	syscall
	
	#i++
	addi $t1,$t1,1
	
	#v[k++]
	#sw $t4,4($t4)
	addi $t4, $t4,4
	
	j FOR_PRINT
FIM_FOR_PRINT:
