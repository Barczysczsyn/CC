.data

	str1: .asciiz "caramujo"
	
	#.align 2
	str2:.asciiz " e viado"
	
.text


lui $s0,0x1001
ori $s0,0x0000

add $a0,$s0,$zero

addi $v0,$zero,4

syscall


lui $s1,0x1001
ori $s1,0x1009

add $a0,$s1,$zero

addi $v0,$zero,4

syscall
