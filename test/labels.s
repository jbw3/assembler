# labels

# line after
label1:
	mov r0, r1

# same line
label2 : addi r2, label1

# 2 lines after
label3	:

	addi r3, label2

# 2 labels
label4:
label5:
	addi r4, label3

	addi r5, label4
	addi r6, label5

# reference label defined later
    addi r7, label6

label6:
    mov r1, r2
