# w = word = 4 bytes
# h = half = 2 bytes
# b = byte = 1 byte

# load
# lw 
# lw $reg-dest,offset[numero]($reg-end-base)
# $reg-dest = MEM[reg_end_base + offset]

# lh
# lb

# store
# sw
# sw $reg-orgigem,offset[numero]($reg-end-base)
# MEM[reg_end_base + offset] = $reg-origem

# sh
# sb

#------------------ Alinhamento de endereços ------------------#

# w - endereço é multiplo de 4
# h - endereço é multiplo de 2
# existe um syscall para alocação dinâmica
# sp começa na PRIMEIRA posição vazia da pilha
# ra marca o endereço de retorno, para o programa voltar pro lugar certo

#int *V; // pois V[10] (por exemplo), é alocado no .data
#int n;
# V -> $s0, n->$s1 e i->$2
#n = 5;
addi $s1,$zero,5

#V = alocar 5 inteiros;
addi $v0,$zero,9
addi $t0,$zero,4 #//sizeof(int)
mul $a0,$s1,$t0 #//sizeof(int)*n
syscall
add $s0,$zero,$v0

#for(i=0;i<n;++i)
add $s2,$zero,$zero
FOR_LEITURA:
	slt $t0,$s2,$s1
	beq $t0,$zero,FIM_FOR_LEITURA
#	leia V[i]
	addi $v0,$zero,5
	syscall
	#//calcular o endereço (V+i*sizeof(int))
	addi $t0,$zero,4
	mul $t0,$s2,$t0
	add $t0,$t0,$s0
	
	sw $v0,0($t0)
	
	addi $s2,$s2,1
	j FOR_LEITURA
FIM_FOR_LEITURA:
#for(i=0;i<n;++i)
#	escreva V[i] + ' '


#for(i=0;i<n;++i)
add $s2,$zero,$zero
FOR_IMPRESSAO:
	slt $t0,$s2,$s1
	beq $t0,$zero,FIM_FOR_IMPRESSAO

	#//calcular o endereço (V+i*sizeof(int))
	addi $t0,$zero,4
	mul $t0,$s2,$t0
	add $t0,$t0,$s0
	
	lw $a0,0($t0)

#	escreva V[i]
	addi $v0,$zero,1
	syscall
			
	addi $s2,$s2,1
	j FOR_IMPRESSAO
FIM_FOR_IMPRESSAO:
#for(i=0;i<n;++i)
#	escreva V[i] + ' '