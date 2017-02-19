num = 2
hex=0xA1

label1:
    mov r1, r2
    mov r2, r3
label2:
    mov r3, r4

lbl = label2
    addi r0, num
    addi r1, hex
    addi r2, lbl
