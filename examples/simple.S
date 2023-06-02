.globl main

.data
m1:	.word 6
m2:	.word 5
m3:	.word 3

.text
main:
	la $t1, m1
	lw $t3, 0($t1)
	la $t0, m2
	lw $t2, 0($t0)
	add $t0, $t3, $t2
	addu $t0, $t2, $t3
	or $t0, $t2, $t3
	lb $t0, 4($t1)
lab:
	la $t1, m3
	or $t1, $t1, $t1
	lb $t0, 8($t1)
	bltz $t2, lab
