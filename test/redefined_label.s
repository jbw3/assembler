# Redefining a label should be an error

label1:
    mov R10, R12

label2:
    not R10, r10

label1:
    mov r12, r10
