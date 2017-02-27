# expressions

PARS = (2 + 2) * 10
BITS = ~(0x18 >> 3)

start:

    addi r0, PARS
    addi r0, SIZE
    addi r0, BITS

    # infinite loop
    nop
    jmp here - 2

    nop

end:
SIZE = end - start
