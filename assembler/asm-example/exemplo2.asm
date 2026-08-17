_start:
	mov r0, 0
	mov r1, 1
	mov r2, 3
	mov r6, 0
loop:
	add r0, r0, r1
	cmp_neq r3, r0, r2
	jump_cond r3, loop
	add r5, r0, r6
	mov r0, 0
	syscall