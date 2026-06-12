
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
	sw $v0,0($t4)
	
	#k++
	addi $t1,$t1,1
	
	#v[k++]
	addi $t4,$t4,4
	##é assim? FIXME


	j FOR_V
FIM_FOR_V:




