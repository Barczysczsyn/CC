
#ler n
addi $v0,$zero,5
syscall

#s0 e n
addi $s0,$zero,$v0

#aloca n bits
addi $a0,$zero,$s0
syscall

#s1 é V
addi $s1,$zero,$v0

