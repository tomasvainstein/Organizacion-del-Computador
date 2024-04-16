
	.text
	
.globl intClone

.globl strCmp
.globl strLen
.globl strDelete

.globl arrayGetSize
.globl arrayAddLast
.globl arrayGet

.globl listNew
.globl listGetSize
.globl listGet
.globl listAddFirst

.globl cardCmp
.globl cardClone
.globl cardAddStacked
.globl cardDelete

# ** Int **

# int32_t* intClone(int32_t* a)
intClone:
	pushq %rbp
	movq %rsp, %rbp
	pushq %rbx

	movq %rdi, %rax
	movl (%rax), %ebx

	movq $4, %rdi
	call malloc

	movq %rax, %rdi
	movl %ebx, (%rax)

	pop %rbx
	pop %rbp
	ret

# ** String **

# int32_t strCmp(char* a, char* b)
strCmp:
    push %rbp
    mov %rsp, %rbp
    sub $8, %rsp
    push %r12
    push %r13
    push %r14
    push %r15
    push %rbx
    push %rcx
    cmp $0, %rdi
    je .finfunc
    cmp $0, %rsi
    je .finfunc

    mov %rdi, %r12
    mov %rsi, %r13

    call strLen
    mov %rax, %r14
    mov %r13, %rdi
    call strLen
    mov %rax, %r15
    inc %r14
    inc %r15
    cmp %r15, %r14
    jle .minA
    mov %r14, %rcx
    mov $0, %rbx

.itera:
    movb (%r12, %rbx), %al
    cmpb (%r13, %rbx), %al
    jg .aEsMayor
    jl .bEsMayor
    inc %rbx
    cmp %rcx, %rbx
    jl .itera

    cmp %r15, %r14
    jg .aEsMayor
    jl .bEsMayor
    mov $0, %rax
    jmp .finfunc

.aEsMayor:
    mov $-1, %rax
    jmp .finfunc

.bEsMayor:
    mov $1, %rax
    jmp .finfunc

.minA:
    mov %r15, %rcx
    mov $0, %rbx
    jmp .itera

.finfunc:
    pop %rcx
    pop %rbx
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    add $8, %rsp
    pop %rbp
    ret

# void strDelete(char* a)
strDelete:
	jmp free
	
# uint32_t strLen(char* a)
strLen:
	cmp $0, %rdi
	je .finalizo
	push %rbp
	mov %rsp, %rbp
	mov $0, %rcx
	.bucle:
		cmpb $0, (%rdi, %rcx)
		je .finito
		inc %rcx
		jmp .bucle          
	.finito:
		mov %rcx, %rax
		pop %rbp
	.finalizo:
    	ret

# ** Array **

# uint8_t  arrayGetSize(array_t* a)
arrayGetSize:
	cmp $0, %rdi
	je .retorna
	mov 4(%rdi), %al
	.retorna:
		ret

# void  arrayAddLast(array_t* a, void* data)
arrayAddLast:
    push %rbp
    mov %rsp, %rbp
    push %rbx
    push %r15

	cmp $0, %rdi
	je .finFuncion

    movb 4(%rdi), %r9b
    cmpb 5(%rdi), %r9b
    je .finFuncion

    movq %rsi, %rbx
    movq %rdi, %r15

    movl $0, %edi
    movl (%r15), %edi

    call getCloneFunction

    movq %rbx, %rdi
    call *%rax

    movq 8(%r15), %rbx
    movq $0, %rcx

.repet:
    cmpb %cl, 0x4(%r15)
    je .addN
    inc %cl
    jmp .repet

.addN:
    movq %rax, (%rbx,%rcx,8)
    incb 0x4(%r15)

.finFuncion:
    pop %r15
    pop %rbx
    pop %rbp
    ret


# void* arrayGet(array_t* a, uint8_t i)
arrayGet:
    push %rbp
    mov %rsp, %rbp

    mov %rsi, %r8
    cmpb %r8b, 4(%rdi)
    jle .retCero

    mov 8(%rdi), %r9

    mov (%r9,%rsi,8), %rax
    jmp .retorno

	.retCero:
    mov $0, %rax

	.retorno:
    pop %rbp
    ret
	


# ** Lista **

# list_t* listNew(type_t t)
listNew:
	push %rbp
	mov %rsp, %rbp
	sub $8, %rsp
	push %r12

	mov $0, %r12
	mov %edi, %r12d
	mov $24, %rdi
	call malloc

	mov %r12d, (%rax)
	movb $0, 4(%rax)
	movq $0, 8(%rax)
	movq $0, 16(%rax)

	pop %r12
	add $8, %rsp
	pop %rbp
	ret

# uint8_t  listGetSize(list_t* l)
listGetSize:
	mov 4(%rdi), %al
	ret

# void listAddFirst(list_t* l, void* data)
listAddFirst:
    push %rbp
    mov %rsp, %rbp
	subq $8, %rsp
    push %r12
    push %r13
	push %r14

    movq %rdi, %r12
    movq %rsi, %r13

    movq $24, %rdi
    call malloc
	mov %rax, %r14

    mov (%r12), %edi
    call getCloneFunction
    mov %r13, %rdi
    call %rax

    mov %rax, (%r14)
    movq $0, 8(%r14)
    movq $0, 16(%r14)

    cmpb $0, 4(%r12)
    jne .updateFirst
	je .updateList

.updateFirst:
	mov 8(%r12), %rbx
    movq %r14, 16(%rbx)
    movq %rbx, 8(%r14)
    movq %r14, 8(%r12)
	jmp .termina

.updateList:
	mov %r14, 8(%r12)
	mov %r14, 16(%r12)
	
.termina:
	incb 4(%r12)
	pop %r14
    pop %r13
    pop %r12
	addq $8, %rsp
    pop %rbp
    ret

# void* listGet(list_t* l, uint8_t i)
listGet:
	mov 8(%rdi), %rax
	mov %sil, %cl
	cmp $0, %cl
	jl .term
	cmp $0, %rax
	je .final

.lazo:
	cmp $0, %cl
	je .term
	mov 8(%rax), %rax
	dec %cl
	cmp $0, %rax
	je .final
	jmp .lazo
.term:
	mov (%rax), %rax
.final:
	ret

# ** Card **

# int32_t cardCmp(card_t* a, card_t* b)
cardCmp:
	push %rbp
	mov %rsp, %rbp
	push %r12
	push %r13

	mov %rdi, %r12
	mov %rsi, %r13

	mov (%r12), %rdi
	mov (%r13), %rsi

	call strCmp
	cmp $0, %rax
	jne .cierre

	mov 8(%r12), %rdi
	mov 8(%r13), %rsi
	call intCmp

.cierre:
	pop %r13
	pop %r12
	pop %rbp
	ret

# card_t* cardClone(card_t* c)
cardClone:
    push %rbp
    mov %rsp, %rbp
    push %r12
    push %r13

    mov %rdi, %r12

    mov $24 , %rdi
    call malloc
    mov %rax, %r13

	mov (%r12), %rdi
	call strClone
	mov %rax, %rcx
	mov %rcx, (%r13)

	mov 8(%r12), %rdi
	call intClone
	mov %rax, %rcx
	mov %rcx, 8(%r13)

    mov 16(%r12), %rdi
    call listClone
    mov %rax, %rdx
    mov %rdx, 16(%r13)

    mov %r13, %rax

.finalizacion:
    pop %r13
    pop %r12
    pop %rbp
    ret

# void cardAddStacked(card_t* c, card_t* card)
cardAddStacked:
	push %rbp
	mov %rsp, %rbp
	push %r12
	push %r13

	mov %rdi, %r12
	mov %rsi, %r13

	movq 16(%r12), %rdi
	mov %r13, %rsi
	call listAddFirst

	pop %r13
	pop %r12
	pop %rbp
	ret

# void cardDelete(card_t* c)
cardDelete:
		push %rbp
		mov %rsp, %rbp
		push %r12
		subq $8, %rsp

		mov %rdi, %r12

		mov 16(%r12), %rdi
		call listDelete
	
		mov (%r12), %rdi
		call strDelete
		mov 8(%r12), %rdi
		call intDelete

		mov %r12, %rdi
		call free
	
    	add $8, %rsp
    	pop %r12
    	pop %rbp
		ret
