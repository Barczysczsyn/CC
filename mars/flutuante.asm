.data

	.asciiz "escreva os floats"
	.asciiz "escreva o inteiro"

.text

lui $a0,0x1001
ori $a0, $a0,0x0000

addi $v0,$zero,4
syscall

addi $v0,$zero,6
syscall


#era pra ser zero mas coloquei f10
#add.s $f1,$f10,$f0
mov.s $f1,$f0

addi $v0,$zero,6
syscall


lui $a0,0x1001
ori $a0,$a0,0x0012
addi $v0,$zero,4
syscall

addi $v0,$zero,5
syscall

add $s0,$zero,$v0

add.s $f12,$f0,$f1
mtc1 $s0,$f2
cvt.s.w $f2,$f2
add.s $f12,$f12,$f2

addi $v0,$zero,2
syscall