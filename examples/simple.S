.globl main

.data
m1:	.word 6
m2:	.word 5

.text
main:
	la $t0, m1
	lw $t1, 0($t0)
	la $t0, m2
	lw $t0, 0($t0)
	add $t0, $t1, $t0
, 1
	li $t2, 0
lab:
	add $t2, $t2, $t3
	sub $t0, $t1, $t4
	addi $t1, $t1, 1
	bltz $t0, lab
	la $t0, m3
	sw $t2, 0($t0)
	nop
