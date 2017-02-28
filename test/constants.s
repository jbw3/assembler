num = 2
hex=0xA2

label1:
    mov r1, r2
    mov r2, r3
label2:
    mov r3, r4

lbl = label2
    jmp num
    jmp hex
    jmp lbl
