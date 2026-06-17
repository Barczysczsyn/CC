.data
minha_variavel:  .word 0          # Inicializa um inteiro com valor 0
nome_string:     .asciiz "Olá\n"  # Inicializa uma string terminada em nulo
vetor:           .word 0:5        # Inicializa um vetor com 5 posições zeradas

.text

addi $t0,$zero,5
addi $t1,$zero,2
mul $a0,$t0,$t1

#addi $a0,$zero, 5(2)
addi $v0, $zero,1

syscall