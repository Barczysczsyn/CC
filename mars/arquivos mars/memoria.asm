#w = word/4 bytes
#h = half/2 bytes
#b = byte


#n=5
addi $s1,$zero,5
#v -> $s0,n -> $s1, i -> $s2
#v = aloca 5 inteiros
addi $v0,$zero,9
addi $t0, $zero, 4
mul $a0,$s1,$t0
syscall

addi $s2, $zero,$zero
FOR_LEITURA:
	slt $t0,$s2,$s1
	beq $t0,$zero, FIM_FOR _LEITURA
	
	addi $v0,$zero,5
	syscall
	
	#calcular o endereco
	addi $t0,$zero,4
	mul $t0,$s2,$t0
	add $t0,$t0,$s0
	sw $v0,0($t0)
	addi $s2,$s2,1
	j FOR_LEITURA
	
FIM_FOR_LEITURA:

add $s2, $zero,$zero

FOR_IMPRESSAO:
slt $t0,$zero,4
mul $t0,$s2,$t0
add $t0,$t0,$s0

lw $a0,0($t0)

addi $v0,$zero,1
syscall



addi $s2,$s2,1
j FOR_IMPRESSAO
FIM_FOR_IMPRESSAO: