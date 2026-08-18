data: .data 1
x: .word 3;
str_resultado: .asciiz "resultado: "

_start:
	mov r0, x
	load r1, [r0]         # r1 = x

	mov r0, 5
	add r1, r1, r0         # r1 = x + 5

	mov r0, 2
	sub r1, r1, r0         # r1 = (x+5) - 2

	mov r0, 3
	mul r1, r1, r0         # r1 = ((x+5)-2) * 3

	mov r0, 2
	div r1, r1, r0         # r1 = resultado parcial

	mov r0, 10
	cmp_greater r2, r1, r0
	jump_cond r2, guarda    # se resultado > 10, pula direto pra guardar

	# --- não era maior: soma +1 três vezes ---
	mov r0, 1               # incremento
	mov r3, 0                 # contador do for (i)
	mov r4, 3                   # repete 3 vezes
for_loop:
	add r1, r1, r0           # resultado += 1
	add r3, r3, r0             # i++
	cmp_neq r5, r3, r4            # r5 = (i != 3) ?
	jump_cond r5, for_loop           # continua enquanto i != 3

guarda:
	mov r0, 0
	add r6, r1, r0           # backup r1 (resultado) em r6

	mov r0, data
	store [r0], r1            # guarda o resultado final na memória

	mov r0, 1                  # 1 is the print string
	mov r1, str_resultado
	syscall

	mov r0, 0
	add r1, r6, r0             # restore r1 (resultado)
	mov r0, 3                   # 3 is integer print service
	syscall

	mov r0, 2                     # 2 is newline print service
	syscall

end:
	mov r0, 0
	syscall