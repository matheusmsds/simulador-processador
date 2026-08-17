_start:
	mov r0, 5
	mov r1, 5
	cmp_equal r2, r0, r1
	jump_cond r2, alvo
	mov r3, 111
	syscall
alvo:
	mov r3, 222
	mov r0, 0
	syscall