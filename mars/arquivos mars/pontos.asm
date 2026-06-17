.data

	.asciiz "Ola mundo" #0x10010000"
	.align 2 #0x10010009
	.word 1,2,3,4 #0x1001000c
.text



lui $a0, 0x1001
addi $v0,$zero,4
syscall


#usar pseudoinstrucoes é proibido na prova, vai perder meio ponto por vez
#la $s0,W_ARRAY
lui $s0, 0x1001
ori $s0,$s0,0x000c
lw $a0,0($s0)
addi $v0,$zero,1
syscall