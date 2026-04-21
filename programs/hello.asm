.data
msg:    .asciiz "Hello, World!\n"

.text
        li      $t0, 0x800      # address of msg (DATA_START)
        li      $t1, 0xFFF      # MMIO_OUT address

loop:
        lw      $t2, 0($t0)     # load next character
        beq     $t2, $zero, done
        sw      $t2, 0($t1)     # write character to stdout
        addi    $t0, $t0, 1     # advance to next character
        j       loop

done:
        halt
