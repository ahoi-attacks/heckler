.align 4096
.globl simple_cache_target_lfence_asm
simple_cache_target_lfence_asm:
    nop
    nop
    # rdi is addr of lookup table
    mov rax, 42
    lfence
    mov rdx, [rdi + (1* 64)]
    lfence
    mov rdx, [rdi + (5* 64)]
    lfence
    mov rdx, [rdi + (14* 64)]
    lfence
    ret

.align 4096

.align 4096
.globl simple_cache_target_asm
simple_cache_target_asm:
    nop
    nop
    # rdi is addr of lookup table
    mov rax, 42
    mov rdx, [rdi + (1* 64)]
    mov rdx, [rdi + (5* 64)]
    mov rdx, [rdi + (14* 64)]
    ret

.align 4096


.align 4096
.globl eval_cache_target_asm
eval_cache_target_asm:
    nop
    nop
    # rdi is addr of lookup table
    mov rax, 42

    mov rdx, [rdi + ( 1 * 64)]
    mov rdx, [rdi + (15 * 64)]
    mov rdx, [rdi + ( 1 * 64)]
    mov rdx, [rdi + (15 * 64)]

    mov rdx, [rdi + ( 1 * 64)]
    mov rdx, [rdi + (15 * 64)]
    mov rdx, [rdi + ( 1 * 64)]
    mov rdx, [rdi + (15 * 64)]

    mov rdx, [rdi + ( 1 * 64)]
    mov rdx, [rdi + (15 * 64)]
    mov rdx, [rdi + ( 1 * 64)]
    mov rdx, [rdi + (15 * 64)]

    mov rdx, [rdi + ( 1 * 64)]
    mov rdx, [rdi + (15 * 64)]
    mov rdx, [rdi + ( 1 * 64)]
    mov rdx, [rdi + (15 * 64)]

    mov rdx, [rdi + ( 1 * 64)]
    mov rdx, [rdi + (15 * 64)]
    mov rdx, [rdi + ( 1 * 64)]
    mov rdx, [rdi + (15 * 64)]
    ret
.align 4096



.align 4096
.globl eval_cache_target_lfence_asm
eval_cache_target_lfence_asm:
    nop
    nop
    # rdi is addr of lookup table
    mov rax, 42

    mov rdx, [rdi + ( 1 * 64)]
    lfence
    mov rdx, [rdi + (15 * 64)]
    lfence
    mov rdx, [rdi + ( 1 * 64)]
    lfence
    mov rdx, [rdi + (15 * 64)]
    lfence

    mov rdx, [rdi + ( 1 * 64)]
    lfence
    mov rdx, [rdi + (15 * 64)]
    lfence
    mov rdx, [rdi + ( 1 * 64)]
    lfence
    mov rdx, [rdi + (15 * 64)]
    lfence

    mov rdx, [rdi + ( 1 * 64)]
    lfence
    mov rdx, [rdi + (15 * 64)]
    lfence
    mov rdx, [rdi + ( 1 * 64)]
    lfence
    mov rdx, [rdi + (15 * 64)]
    lfence

    mov rdx, [rdi + ( 1 * 64)]
    lfence
    mov rdx, [rdi + (15 * 64)]
    lfence
    mov rdx, [rdi + ( 1 * 64)]
    lfence
    mov rdx, [rdi + (15 * 64)]
    lfence

    mov rdx, [rdi + ( 1 * 64)]
    lfence
    mov rdx, [rdi + (15 * 64)]
    lfence
    mov rdx, [rdi + ( 1 * 64)]
    lfence
    mov rdx, [rdi + (15 * 64)]
    lfence
    ret
.align 4096

# make sure our function starts on a fresh page
.align 4096
.globl nop_slide_asm
nop_slide_asm:
# 100 nops
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

# 100 nops
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

# 100 nops
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

# 100 nops
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

# 100 nops
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

# 100 nops
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

# 100 nops
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

# 100 nops
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

# 100 nops
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

# 100 nops
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop


    ret
    # make sure no other code is placed on the remainder of this page
    .align 4096


# NEMESIS benchmarks

.align 4096
.globl add_slide_asm
add_slide_asm:
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1

    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1

    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1

    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1

    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1

    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1

    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1

    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1

    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1

    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1
    add rax,1

    ret
.align 4096

.align 4096
.globl rcl_slide_asm
rcl_slide_asm:

    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0
    rcl r8d,0x0

    ret
.align 4096

.align 4096
.globl lar_slide_asm
lar_slide_asm:
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w
    lar r9,r8w

    ret
.align 4096

.align 4096
.globl rdrand32_slide_asm
rdrand32_slide_asm:
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax
    rdrand eax

    ret


.align 4096
.globl rdrand_slide_asm
rdrand_slide_asm:
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax

    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax

    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax

    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax

    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax

    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax

    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax

    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax

    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax

    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax
    rdrand rax

    ret
.align 4096

.align 4096
.globl div_slide_asm_r64
div_slide_asm_r64:
    # 100
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    
    # 70
        mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0x0000000000000000
    mov rbx, 0xffffffffffffffff
    div rbx

    ret
.align 4096

.align 4096
.globl div_slide1_asm_r64
div_slide1_asm_r64:
    # 100
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    
    # 70
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0000000000000000
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx

    ret
.align 4096

.align 4096
.globl div_slide2_asm_r64
div_slide2_asm_r64:
    # 100
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx

    # 70
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x00000000ffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    ret
.align 4096

.align 4096
.globl div_slide3_asm_r64
div_slide3_asm_r64:
    # 150
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx
    mov rdx, 0x0fffffffffffffff
    mov rax, 0xffffffffffffffff
    mov rbx, 0xffffffffffffffff
    div rbx

    ret
.align 4096

.align 4096
.globl div_slide4_asm_r64
div_slide4_asm_r64:
    # 136
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx
    mov rdx, 0x343a9ed744556677
    mov rax, 133
    mov rbx, 0x75e6e44fccddeeff
    div rbx

    ret
.align 4096

.align 4096
.globl div_slide_asm_r32
div_slide_asm_r32:
    # 240
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    mov edx,0x343a9ed7
    mov eax,0x85
    mov ebx,0x75e6e44f
    div ebx
    
    ret

.align 4096

.align 4096
.globl div_slide_asm_r16
div_slide_asm_r16:
    # 227
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    mov edx,0x343a
    mov eax,0x85
    mov ebx,0x75e6
    div bx
    
    ret

.align 4096


.align 4096
.globl mul_slide_asm_r64
mul_slide_asm_r64:
    mov r8, 0x0000000000000002

    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8

    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8

    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8

    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8

    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8

    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8

    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8

    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8

    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8

    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8
    mul r8

    ret
    .align 4096
