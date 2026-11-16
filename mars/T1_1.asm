j COMECO
#FUNCAO PINTAR DISCO
#leva em conta que se esta no comeco da estaca
#a0 é o tamanho do  disco
#$s1 é o ponteiro no display

FUNCAO_PINTAR_DISCOS:
	#guarda o valor em $t0

	#t0 = n
	add $t7,$zero,$s4
	INICIO_FOR_PINTAR_DISCOS:
	beq $t7,$zero,FIM_FOR_PINTAR_DISCOS
	
		
		#chamada de funcao
		#salva ra
		addi $sp,$sp,-4
		sw $ra,0($sp)
		
		#
		lw $a0,4($s7)
		jal FUNCAO_PINTAR_DISCO
	
		#restaura a pilha
		lw $ra,0($sp)
		addi $sp,$sp,4
		
		#pula pra proxima estaca
		addi $s1,$s1,504
		
		
		#chamada de funcao
		#salva ra
		addi $sp,$sp,-4
		sw $ra,0($sp)
		
		#
		lw $a0,4($s6)
		jal FUNCAO_PINTAR_DISCO
	
		#restaura a pilha
		lw $ra,0($sp)
		addi $sp,$sp,4
		
		#pula pra proxima estaca
		addi $s1,$s1,504
		
		
		#chamada de funcao
		#salva ra
		addi $sp,$sp,-4
		sw $ra,0($sp)
		
		#
		lw $a0,4($s5)
		jal FUNCAO_PINTAR_DISCO
	
		#restaura a pilha
		lw $ra,4($sp)
		addi $sp,$sp,4
		
		#pula pra proxima estaca
		addi $s1,$s1,1016
		
		addi $t7,$t7,-1
		j INICIO_FOR_PINTAR_DISCOS
	FIM_FOR_PINTAR_DISCOS:


jr $ra



FUNCAO_PINTAR_DISCO:

#o tamanho do disco vai ser 8 + (4 * a0)
addi $t0,$zero,4
mul $t1, $t0,$a0
addi $a0,$t1,8


addi $s0,$zero, 0xfc0000	#a cor a ser pintada
#lui $s1,0x1001		#endereco base do array ( = .data)

addi $t5,$zero,5

INICIO_PINTAR_DISCO_VERTICAL:

	beq $t5,$zero,FIM_PINTAR_DISCO_VERTICAL
	
	
	#volta um pouco para tras
	addi $t0,$zero,4
	mul $t1,$a0,$t0
	add $s1,$s1,$t1
	#o dobro do tamanho pra fazer os dois lados do disco
	addi $t0,$zero,2
	mul $t4,$t0,$a0
	#nao sei exatamente porque nao é 2048
	addi $t2,$zero,-2032
	add $t2,$t2,$t4

	INICIO_PINTAR_DISCO_HORIZONTAL:
		beq $t4,$zero,FIM_PINTAR_DISCO_HORIZONTAL
	
	
		sw $s0,0($s1)
		addi $s1,$s1,-4
	

		addi $t4,$t4,-1
		j INICIO_PINTAR_DISCO_HORIZONTAL
	FIM_PINTAR_DISCO_HORIZONTAL:
	
	#pula uma linha pra cima
	add $s1,$s1,$t2
	addi $t5,$t5,-1
	j INICIO_PINTAR_DISCO_VERTICAL
FIM_PINTAR_DISCO_VERTICAL:
jr $ra


DESENHAR_TELA:


#limpa a tela
addi $s0,$zero, 0x000000	#a cor a ser pintada
lui $s1,0x1001		#endereco base do array ( = .data)
addi $t0,$zero,131072

LIMPA_TELA:
	beq $t0,$zero, FIM_LIMPA_TELA
	sw $s0,0($s1)
	addi $s1,$s1,4
	
	addi $t0,$t0,-1
j LIMPA_TELA
FIM_LIMPA_TELA:

#desenha as estacas
addi $s0,$zero, 0x5e3d15	#a cor a ser pintada
lui $s1,0x1001		#endereco base do array ( = .data)
addi $s2,$zero,204800  #2048 * 400
addi $s2,$s2,400
add $s1,$s2,$s1
add $t4,$zero,120

INICIO_ESTACA:
	beq $t4,$zero,FIM_ESTACA
	#pintar
	sw $s0,0($s1)
	addi $s1,$s1,4
	sw $s0,0($s1)
	addi $s1,$s1,4
	sw $s0,0($s1)
	#pula pra proxima estaca
	addi $s1,$s1,504
	sw $s0,0($s1)
	addi $s1,$s1,4
	sw $s0,0($s1)
	addi $s1,$s1,4
	sw $s0,0($s1)
	#pula pra proxima estaca
	addi $s1,$s1,504
	sw $s0,0($s1)
	addi $s1,$s1,4
	sw $s0,0($s1)
	addi $s1,$s1,4
	sw $s0,0($s1)
	#pula pra proxima estaca
	addi $s1,$s1,1016
	#pintar
	addi $t4,$t4,-1
	j INICIO_ESTACA	
FIM_ESTACA:

#desenha os numeros
addi $s1,$s1,12288
add $t4,$zero,16 #altura do numero
	addi $s0,$zero, 0xffffff	#a cor a ser pintada
INICIO_NUMERO:

	beq $t4,$zero,FIM_NUMERO
	
	sw $s0,0($s1)
	addi $s1,$s1,512
	
	sw $s0,0($s1)
	addi $s1,$s1,16
	sw $s0,0($s1)
	addi $s1,$s1,496
	
	
	sw $s0,0($s1)
	addi $s1,$s1,16
	sw $s0,0($s1)
	addi $s1,$s1,16
	sw $s0,0($s1)
	addi $s1,$s1,992
	
	
	
	addi $t4,$t4,-1
	
	j INICIO_NUMERO
FIM_NUMERO:

#TODO pular pra estaca, esta no numero
addi $s1,$s1, -32768
addi $s1,$s1,-12288
addi $a0,$zero,8

	#chamada de funcao
	#salva ra
	addi $sp,$sp,-4
	sw $ra,4($sp)
		
jal FUNCAO_PINTAR_DISCOS

		#restaura a pilha
		lw $ra,4($sp)
		addi $sp,$sp,4
		
#volta pra onde foi chamado

LOOP_ESPERA:
	#espera o usuario apertar espaco
	li $v0, 12
	syscall
	li $t0, 32
	bne $v0, $t0, LOOP_ESPERA
	
jr $ra


# FUNCAO_HANOI
# Argumentos:
# $a0 = n (número de discos)
# $a1 = origem
# $a2 = destino
# $a3 = auxiliar

FUNCAO_HANOI:
    # Caso base: se n == 1, move o disco diretamente e retorna
    addi $t0, $zero, 1
    beq  $a0, $t0, BASE_HANOI

    # --- CASO RECURSIVO ---
    # Precisamos salvar $ra e os argumentos originais na pilha
    addi $sp, $sp, -20
    sw   $ra, 16($sp)
    sw   $a0, 12($sp)
    sw   $a1, 8($sp)
    sw   $a2, 4($sp)
    sw   $a3, 0($sp)

    # PASSO 1: Mover n-1 discos de 'origem' para 'auxiliar' usando 'destino'
    addi $a0, $a0, -1   # n = n - 1
    # Troca de papéis: Destino vira Auxiliar, Auxiliar vira Destino
    # Novo destino ($a2) = antigo auxiliar ($a3)
    # Novo auxiliar ($a3) = antigo destino ($a2)
    move $t0, $a2
    move $a2, $a3
    move $a3, $t0
    jal  FUNCAO_HANOI
    
    #imprime a tela, salva o a0 e o pega de novo
    #add $t5, $zero,$a0
    #add $a0
    jal DESENHAR_TELA

    # Restaurar o estado original após a primeira chamada recursiva
    lw   $a3, 0($sp)
    lw   $a2, 4($sp)
    lw   $a1, 8($sp)
    lw   $a0, 12($sp)

    # PASSO 2: Mover o disco restante (disco n) de 'origem' para 'destino'
    lw   $t3, 0($a1)     # Lê o disco do topo da origem
    addi $a1, $a1, 4     # Remove da origem (desempilha da torre origem)
    addi $a2, $a2, -4    # Abre espaço no destino (empilha na torre destino)
    sw   $t3, 0($a2)     # Grava o disco no destino
    
    
    #imprime a tela, salva o a0 e o pega de novo
    #add $t5, $zero,$a0
    #add $a0
    jal DESENHAR_TELA

    # PASSO 3: Mover n-1 discos de 'auxiliar' para 'destino' usando 'origem'
    addi $a0, $a0, -1   # n = n - 1
    # Troca de papéis: Origem vira Auxiliar, Auxiliar vira Origem
    # Nova origem ($a1) = antigo auxiliar ($a3)
    # Novo auxiliar ($a3) = antiga origem ($a1)
    move $t0, $a1
    move $a1, $a3
    move $a3, $t0
    jal  FUNCAO_HANOI
    
    
    #imprime a tela, salva o a0 e o pega de novo
    #add $t5, $zero,$a0
    #add $a0
    jal DESENHAR_TELA

    # Finalizar caso recursivo: restaurar $ra e limpar a pilha
    lw   $ra, 16($sp)
    addi $sp, $sp, 20
    jr   $ra

BASE_HANOI:
    # --- CASO BASE (n == 1) ---
    # Move apenas 1 disco da origem para o destino diretamente
    lw   $t3, 0($a1)     # Pega o elemento do topo da origem
    addi $a1, $a1, 4     # Avança o ponteiro da origem (remove)
    addi $a2, $a2, -4    # Recua o ponteiro do destino (abre espaço)
    sw   $t3, 0($a2)     # Salva no destino
    jr   $ra             # Retorna


COMECO:

#pega o valor de N
addi $v0,$zero,5
syscall
#n é s4
add $s4,$zero,$v0
add $a0,$zero,$s4

#cada estaca tem espaço para N discos
#aloca memoria da primeira estaca
addi $v0,$zero,9
syscall
add $s5,$zero,$v0

#aloca memoria da segunda estaca
addi $v0,$zero,9
syscall
add $s6,$zero,$v0

#aloca memoria da terceira estaca
addi $v0,$zero,9
syscall
add $s7,$zero,$v0

#coloca todos os discos na estaca 1
addi $t0,$zero,1
INICIALIZA_TORRE:
#vai até t5 (n+1)
addi $t5,$s0,1
beq $t0,$t5,FIM_INICIALIZA_TORRE

	#coloca o disco na torre
	addi $s5,$s5,-4
	sw $t0,4($s5)

	addi $t0,$t0,1
	j INICIALIZA_TORRE
FIM_INICIALIZA_TORRE:


jal DESENHAR_TELA

#coloca os argumentos
add $a0, $zero,$s4
add $a1, $zero,$s5
add $a2, $zero,$s7
add $a3, $zero,$s6
jal FUNCAO_HANOI