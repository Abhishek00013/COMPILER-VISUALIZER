.section .data
a: .long 0
4: .long 0
b: .long 0
3: .long 0
t0: .long 0
t1: .long 0
2: .long 0
c: .long 0
t2: .long 0
d: .long 0
5: .long 0
1: .long 0
t3: .long 0
20: .long 0

.section .text
.global _start
_start:
    # a = 4
    movl $4, %eax
    movl %eax, a
    # b = 3
    movl $3, %eax
    movl %eax, b
    # t0 = a + b
    movl a, %eax
    addl b, %eax
    movl %eax, t0
    # t1 = t0 * 2
    movl t0, %eax
    imull $2, %eax
    movl %eax, t1
    # c = t1
    movl t1, %eax
    movl %eax, c
    # t2 = c / 2
    movl c, %eax
    movl %eax, %ebx
    movl $2, %ecx
    xorl %edx, %edx
    idivl %ecx
    movl %eax, %ebx
    movl %ebx, %eax
    movl %eax, t2
    # d = t2
    movl t2, %eax
    movl %eax, d
    # if d > 5 goto L0
    movl d, %eax
    cmpl 5, %eax
    jg L0
 # goto L1
 jmp L1
L0:
    # d = d - 1
    movl d, %eax
    subl $1, %eax
    movl %eax, d
 # goto L2
 jmp L2
L1:
    # t3 = d + 1
    movl d, %eax
    addl $1, %eax
    movl %eax, t3
    # d = t3
    movl t3, %eax
    movl %eax, d
L2:
    # if d < 20 goto L3
    movl d, %eax
    cmpl 20, %eax
    jl L3
 # goto L4
 jmp L4
L3:
    # d = d + 1
    movl d, %eax
    addl $1, %eax
    movl %eax, d
 # goto L2
 jmp L2
L4:
    # return d
    movl d, %eax
    movl $1, %ebx
    int $0x80
