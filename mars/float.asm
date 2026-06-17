addi $v0,$zero,6
syscall
#$f1 nao é inicializado
add.s $f1,$f1,$f0

syscall
#add.s $s1,$zero,$f0

add.s $f12,$f0,$f1

addi $v0,$zero,2

syscall