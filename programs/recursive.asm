main:
	addi $a0, $0, 4 # set arg
	jal factorial   # compute the factorial
	move $s0, $v0   # save result
	move $a0, $s0
	jal print_int
	li $a0, 10      # newline
	jal print_char
	j end
factorial:
	addi $sp, $sp, -8 # make room on stack
	sw $a0, 4($sp) # store $a0
	sw $ra, 0($sp) # store $ra
	addi $t0, $0, 2 # $t0 = 2
	slt $t0, $a0, $t0 # a <= 1 ?
	beq $t0, $0, else # no - goto else
	addi $v0, $0, 1 # yes - return 1
	addi $sp, $sp, 8 # restore $sp
	jr $ra # return
else:
	addi $a0, $a0, -1 # n = n - 1
	jal factorial # recursive call
	lw $ra, 0($sp) # restore $ra
	lw $a0, 4($sp) # restore $a0
	addi $sp, $sp, 8 # restore $sp
	multu $a0, $v0 # n * factorial(n-1)
	mflo $v0 # mv result into $v0
	jr $ra
end:
	halt

# Added to print values/results
#  print_int($a0): print $a0 as a decimal integer 
print_int:
	addi $sp, $sp, -3
	sw   $ra, 2($sp)
	sw   $s0, 1($sp)
	sw   $s1, 0($sp)
	move $s0, $a0
	li   $s1, 0
	bne  $s0, $zero, pi_extract
	li   $a0, 48
	jal  print_char
	j    pi_done
pi_extract:
	beq  $s0, $zero, pi_print
	move $a0, $s0
	jal  div10
	move $s0, $v0
	addi $v1, $v1, 48
	addi $sp, $sp, -1
	sw   $v1, 0($sp)
	addi $s1, $s1, 1
	j    pi_extract
pi_print:
	beq  $s1, $zero, pi_done
	lw   $a0, 0($sp)
	addi $sp, $sp, 1
	jal  print_char
	addi $s1, $s1, -1
	j    pi_print
pi_done:
	lw   $s1, 0($sp)
	addi $sp, $sp, 1
	lw   $s0, 0($sp)
	addi $sp, $sp, 1
	lw   $ra, 0($sp)
	addi $sp, $sp, 1
	jr   $ra

# div10($a0) → $v0=quotient, $v1=remainder
div10:
	li   $v0, 0
div10_loop:
	slti $t9, $a0, 10
	bne  $t9, $zero, div10_done
	addi $a0, $a0, -10
	addi $v0, $v0, 1
	j    div10_loop
div10_done:
	move $v1, $a0
	jr   $ra

# print_char($a0): write one character to MMIO_OUT
print_char:
	li   $t8, 0xFFF
	sw   $a0, 0($t8)
	jr   $ra
