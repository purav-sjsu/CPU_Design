.text
main:
    li      $t0, 0          # a = current fib
    li      $t1, 1          # b = next fib
    li      $t2, 15         # count

fib_loop:
    beq     $t2, $zero, fib_done

    move    $a0, $t0
    jal     print_int

    li      $a0, 10         # newline
    jal     print_char

    add     $t3, $t0, $t1   # temp = a + b
    move    $t0, $t1        # a = b
    move    $t1, $t3        # b = temp

    addi    $t2, $t2, -1
    j       fib_loop

fib_done:
    halt

# ── print_int($a0): print $a0 as a decimal integer ───────────────────────────
# Saves $ra, $s0 ($a0), $s1 (digit count) on stack.
# Pushes ASCII digit words for each decimal digit, then pops and prints them.
print_int:
    addi    $sp, $sp, -4
    sw      $ra, 0($sp)
    addi    $sp, $sp, -4
    sw      $s0, 0($sp)
    addi    $sp, $sp, -4
    sw      $s1, 0($sp)

    move    $s0, $a0        # n
    li      $s1, 0          # digit count

    bne     $s0, $zero, pi_extract
    li      $a0, 48         # special-case: print '0'
    jal     print_char
    j       pi_done

pi_extract:
    beq     $s0, $zero, pi_print
    move    $a0, $s0
    jal     div10           # $v0 = quotient, $v1 = remainder
    move    $s0, $v0
    addi    $v1, $v1, 48    # digit → ASCII
    addi    $sp, $sp, -4
    sw      $v1, 0($sp)
    addi    $s1, $s1, 1
    j       pi_extract

pi_print:
    beq     $s1, $zero, pi_done
    lw      $a0, 0($sp)
    addi    $sp, $sp, 4
    jal     print_char
    addi    $s1, $s1, -1
    j       pi_print

pi_done:
    lw      $s1, 0($sp)
    addi    $sp, $sp, 4
    lw      $s0, 0($sp)
    addi    $sp, $sp, 4
    lw      $ra, 0($sp)
    addi    $sp, $sp, 4
    jr      $ra

# ── div10($a0): divide by 10 via repeated subtraction ────────────────────────
# Returns: $v0 = quotient, $v1 = remainder. Leaf function, no stack usage.
div10:
    li      $v0, 0
div10_loop:
    slti    $t9, $a0, 10
    bne     $t9, $zero, div10_done
    addi    $a0, $a0, -10
    addi    $v0, $v0, 1
    j       div10_loop
div10_done:
    move    $v1, $a0
    jr      $ra

# ── print_char($a0): write one character to MMIO_OUT ─────────────────────────
print_char:
    li      $t8, 0xFFF
    sw      $a0, 0($t8)
    jr      $ra
