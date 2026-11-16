# =============================================================================
# TORRE DE HANOI - MIPS / MARS (Registradores Limitados)
# Registradores Globais:
# $s4 = N (Número de discos)
# $s0 = Base Torre 1
# $s1 = Base Torre 2
# $s2 = Base Torre 3
# $s5 = Topo Torre 1
# $s6 = Topo Torre 2
# $s7 = Topo Torre 3
# =============================================================================

.text
.globl main

main:
    # Ler N
    addi $v0, $zero, 5
    syscall
    add $s4, $zero, $v0

    # Alocar Memória (N * 4 bytes por torre)
    sll $a0, $s4, 2
    addi $v0, $zero, 9
    syscall
    add $s0, $zero, $v0
    add $s5, $zero, $v0
    
    sll $a0, $s4, 2
    addi $v0, $zero, 9
    syscall
    add $s1, $zero, $v0
    add $s6, $zero, $v0
    
    sll $a0, $s4, 2
    addi $v0, $zero, 9
    syscall
    add $s2, $zero, $v0
    add $s7, $zero, $v0

    # Inicializar Torre 1
    add $t0, $zero, $zero
    add $t1, $zero, $s4
    
    init_loop:
    beq $t0, $t1, init_end
    sub $t2, $s4, $t0
    sw $t2, 0($s5)
    addi $s5, $s5, 4
    addi $t0, $t0, 1
    j init_loop
    
    init_end:
    
    # Chamar Hanoi
    add $a0, $zero, $s4
    add $a1, $zero, $s5
    add $a2, $zero, $s7
    add $a3, $zero, $s6
    jal FUNCAO_HANOI
    
    addi $v0, $zero, 10
    syscall

# =============================================================================
# FUNCAO_HANOI
# =============================================================================
FUNCAO_HANOI:
    addi $t0, $zero, 1
    bne $a0, $t0, ELSE_BASE
    
    # Caso Base: identificar e mover
    beq $a1, $s5, SRC_T1
    beq $a1, $s6, SRC_T2
    j SRC_T3
    
    SRC_T1:
        addi $t1, $s5, -4
        lw $t2, 0($t1)
        addi $s5, $s5, -4
        j FIND_DST
    SRC_T2:
        addi $t1, $s6, -4
        lw $t2, 0($t1)
        addi $s6, $s6, -4
        j FIND_DST
    SRC_T3:
        addi $t1, $s7, -4
        lw $t2, 0($t1)
        addi $s7, $s7, -4
        
    FIND_DST:
        beq $a2, $s5, DST_T1
        beq $a2, $s6, DST_T2
        j DST_T3
        
    DST_T1:
        sw $t2, 0($s5)
        addi $s5, $s5, 4
        j DRAW_BASE
    DST_T2:
        sw $t2, 0($s6)
        addi $s6, $s6, 4
        j DRAW_BASE
    DST_T3:
        sw $t2, 0($s7)
        addi $s7, $s7, 4
        
    DRAW_BASE:
        jal DESENHAR_TELA
        addi $v0, $zero, 32
        addi $a0, $zero, 100
        syscall
        jr $ra

    ELSE_BASE:
        addi $sp, $sp, -20
        sw $ra, 16($sp)
        sw $a0, 12($sp)
        sw $a1, 8($sp)
        sw $a2, 4($sp)
        sw $a3, 0($sp)
        
        # Passo 1
        addi $a0, $a0, -1
        add $t0, $zero, $a2
        add $a2, $zero, $a3
        add $a3, $zero, $t0
        jal FUNCAO_HANOI
        
        # Passo 2
        lw $a0, 12($sp)
        lw $a1, 8($sp)
        lw $a2, 4($sp)
        lw $a3, 0($sp)
        
        beq $a1, $s5, REC_SRC_T1
        beq $a1, $s6, REC_SRC_T2
        j REC_SRC_T3
        
        REC_SRC_T1:
            addi $t1, $s5, -4
            lw $t2, 0($t1)
            addi $s5, $s5, -4
            j REC_FIND_DST
        REC_SRC_T2:
            addi $t1, $s6, -4
            lw $t2, 0($t1)
            addi $s6, $s6, -4
            j REC_FIND_DST
        REC_SRC_T3:
            addi $t1, $s7, -4
            lw $t2, 0($t1)
            addi $s7, $s7, -4
            
        REC_FIND_DST:
            beq $a2, $s5, REC_DST_T1
            beq $a2, $s6, REC_DST_T2
            j REC_DST_T3
            
        REC_DST_T1:
            sw $t2, 0($s5)
            addi $s5, $s5, 4
            j REC_DRAW
        REC_DST_T2:
            sw $t2, 0($s6)
            addi $s6, $s6, 4
            j REC_DRAW
        REC_DST_T3:
            sw $t2, 0($s7)
            addi $s7, $s7, 4
            
        REC_DRAW:
            jal DESENHAR_TELA
            addi $v0, $zero, 32
            addi $a0, $zero, 100
            syscall
            
        # Passo 3
        lw $a0, 12($sp)
        addi $a0, $a0, -1
        lw $t1, 8($sp)
        lw $t2, 4($sp)
        lw $t3, 0($sp)
        add $a1, $zero, $t3
        add $a2, $zero, $t2
        add $a3, $zero, $t1
        jal FUNCAO_HANOI
        
        lw $ra, 16($sp)
        addi $sp, $sp, 20
        jr $ra

# =============================================================================
# DESENHAR_TELA (Otimizado para usar menos registradores)
# =============================================================================
DESENHAR_TELA:
    # Limpar tela
    addi $t0, $zero, 0
    lui $t1, 0x1001
    addi $t2, $zero, 131072
    
    clear_loop:
    beq $t2, $zero, clear_done
    sw $t0, 0($t1)
    addi $t1, $t1, 4
    addi $t2, $t2, -1
    j clear_loop
    
    clear_done:
    
    # Desenhar estacas
    addi $t0, $zero, 0x5e3d15
    
    # Estaca 1
    lui $t1, 0x1001
    addi $t2, $zero, 204800
    addi $t2, $t2, 400
    add $t1, $t1, $t2
    addi $t3, $zero, 120
    
    stake1:
    beq $t3, $zero, stake1_end
    sw $t0, 0($t1)
    addi $t1, $t1, 2048
    addi $t3, $t3, -1
    j stake1
    stake1_end:
    
    # Estaca 2
    lui $t1, 0x1001
    addi $t2, $zero, 204800
    addi $t2, $t2, 1024
    add $t1, $t1, $t2
    addi $t3, $zero, 120
    
    stake2:
    beq $t3, $zero, stake2_end
    sw $t0, 0($t1)
    addi $t1, $t1, 2048
    addi $t3, $t3, -1
    j stake2
    stake2_end:
    
    # Estaca 3
    lui $t1, 0x1001
    addi $t2, $zero, 204800
    addi $t2, $t2, 1648
    add $t1, $t1, $t2
    addi $t3, $zero, 120
    
    stake3:
    beq $t3, $zero, stake3_end
    sw $t0, 0($t1)
    addi $t1, $t1, 2048
    addi $t3, $t3, -1
    j stake3
    stake3_end:
    
    # Desenhar discos
    add $a0, $zero, $s0
    add $a1, $zero, $s5
    addi $a2, $zero, 100
    jal DRAW_DISCS
    
    add $a0, $zero, $s1
    add $a1, $zero, $s6
    addi $a2, $zero, 256
    jal DRAW_DISCS
    
    add $a0, $zero, $s2
    add $a1, $zero, $s7
    addi $a2, $zero, 412
    jal DRAW_DISCS
    
    jr $ra

# =============================================================================
# DRAW_DISCS (Otimizado)
# =============================================================================
DRAW_DISCS:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    
    sub $t0, $a1, $a0
    addi $t1, $zero, 4
    div $t0, $t1
    mflo $t0
    
    beq $t0, $zero, draw_done
    
    addi $t1, $zero, 0
    
    draw_loop:
    beq $t1, $t0, draw_done
    
    # Obter disco
    mul $t2, $t1, 4
    add $t3, $a0, $t2
    lw $t4, 0($t3)
    
    # Largura
    addi $t5, $zero, 4
    mul $t6, $t5, $t4
    addi $t6, $t6, 8
    
    # X inicial
    srl $t7, $t6, 1
    sub $t8, $a2, $t7
    sll $t8, $t8, 2
    
    # Y
    mul $t9, $t1, 8
    addi $t0, $zero, 200
    sub $t0, $t0, $t9
    
    # Endereço
    mul $t1, $t0, 2048
    lui $t0, 0x1001
    add $t0, $t0, $t1
    add $t0, $t0, $t8
    
    # Desenhar
    addi $t1, $zero, 5
    sll $t2, $t6, 2
    
    draw_v:
    beq $t1, $zero, draw_next
    addi $t3, $zero, 0
    
    draw_h:
    beq $t3, $t6, draw_row
    addi $t4, $zero, 0xfc0000
    sw $t4, 0($t0)
    addi $t0, $t0, 4
    addi $t3, $t3, 1
    j draw_h
    
    draw_row:
    addi $t0, $t0, 2048
    sub $t0, $t0, $t2
    addi $t1, $t1, -1
    j draw_v
    
    draw_next:
    addi $t1, $t1, 1
    j draw_loop
    
    draw_done:
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra