; ; ; ; ; ; ; ; ; ; ;

; Pruebas para declaraciones

; Inicia el codigo

.text

; Diversos procedimientos
; Pruebas de operaciones binarias y todos los registros

proc1:	add $r0, $r1, $r2
        sub $r3, $r4, $r5
        mul $r6, $r7, $a0
        div $a1, $s0, $ra
        fadd $pc, $sp, $r0
        fsub $r0, $r1, $r2
        fmul $r6, $r7, $a0
        fdiv $a1, $s0, $ra
	li $r0, proc2

proc2: 	beqz $r0, $r1
       	bltz $r1, $r2
       	li $r2, main
	mov $r0, $s0
       	b $r2

main:	lb $r0, $ra
       	lw $r2, $s0
       	sw $r2, $s0
       	sb $r2, $s0
 	and $ra, $pc, $sp
 	or $ra, $pc, $sp
 	xor $ra, $pc, $sp
 	not $ra, $pc
	syscall
	li $r2, 352
