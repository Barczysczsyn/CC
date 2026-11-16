.data
	.asciiz "eu amo as quintuplas"
	
.text
#para printar a string
lui $s2,0x1001
ori $s2,0x0000

#le um inteiro do usuario
addi $v0,$zero,5
syscall
add $s0,$zero,$v0

#coloca o $s0 no argumento
add $a0,$zero,$s0
jal FUNC_PRINT






FUNC_PRINT:
add $s1,$zero,$zero
slt $t0,$s1,$a0
beq $t0,$zero,FIM_FUNC_PRINT


#printa a string
add $a0,$s2,$zero
addi $v0, $zero, 4
syscall



FIM_FUNC_PRINT:

#volta pra chamada da funcao
jr $ra