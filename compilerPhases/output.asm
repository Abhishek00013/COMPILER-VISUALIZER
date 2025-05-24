.section .data
x: .long 0
10: .long 0
y: .long 0
5: .long 0
t0: .long 0
z: .long 0
t1: .long 0
20: .long 0
2: .long 0

.section .text
.global _start
_start:
    # x = 10
    movl $10, %eax
    movl %eax, x
    # y = 5
    movl $5, %eax
    movl %eax, y
    # t0 = x - y
    movl x, %eax
    subl y, %eax
    movl %eax, t0
    # z = t0
    movl t0, %eax
    movl %eax, z
    # if z < 10 goto L0
    movl z, %eax
    cmpl 10, %eax
    jl L0
 # goto L1
 jmp L1
L0:
    # z = z + 5
    movl z, %eax
    addl $5, %eax
    movl %eax, z
 # goto L2
 jmp L2
L1:
    # t1 = z - 5
    movl z, %eax
    subl $5, %eax
    movl %eax, t1
    # z = t1
    movl t1, %eax
    movl %eax, z
L2:
    # if z < 20 goto L3
    movl z, %eax
    cmpl 20, %eax
    jl L3
 # goto L4
 jmp L4
L3:
    # z = z + 2
    movl z, %eax
    addl $2, %eax
    movl %eax, z
 # goto L2
 jmp L2
L4:
    # return z
    movl z, %eax
    movl $1, %ebx
    int $0x80
