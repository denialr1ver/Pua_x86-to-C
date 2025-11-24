section .data
    multiplier dd 0.00392156862 ; 1 / 255

section .text
bits 64
default rel

global imgCvtGrayInttoFloat

imgCvtGrayInttoFloat:
    ; RCX = height
    ; RDX = width
    ; R8  = input pointer
    ; R9  = output pointer

    imul rcx, rdx ; size / counter

    test rcx, rcx
    jz finish ; if size 0 then done

    movss xmm1, [multiplier] ; xmm1 = 1/255

    ; do 4 pixels at a time for optimization
    mov rax, rcx
    shr rax, 2
    jz .cleanup_start

.unrolled_loop:
    ; pixel1
    movzx r10d, byte [r8]     ; read input
    cvtsi2ss xmm0, r10d       ; convert to float
    mulss xmm0, xmm1          ; multiply by 1/255
    movss [r9], xmm0          ; store result

    ; pixel2
    movzx r11d, byte [r8+1]   ; read input
    cvtsi2ss xmm2, r11d       ; convert to float
    mulss xmm2, xmm1          ; multiply
    movss [r9+4], xmm2        ; store result

    ; pixel3
    movzx r10d, byte [r8+2]   ; read input
    cvtsi2ss xmm3, r10d       ; convert to float
    mulss xmm3, xmm1          ; multiply
    movss [r9+8], xmm3        ; store result

    ; pixel4
    movzx r11d, byte [r8+3]   ; read input
    cvtsi2ss xmm4, r11d       ; convert to float
    mulss xmm4, xmm1          ; multiply
    movss [r9+12], xmm4       ; store result

    ; move pointers
    add r8, 4                 ; input moves 4 bytes
    add r9, 16                ; output moves 16 bytes

    dec rax
    jnz .unrolled_loop

    ; clean remainder
.cleanup_start:
    and rcx, 3        ; get the remainder
    jz finish         ; if no remainder, done

.loop_start:
    ; read input
    movzx eax, byte [r8]

    ; convert to float
    cvtsi2ss xmm0, eax

    ; multiply by 1/255
    mulss xmm0, xmm1

    ; store result in output matrix
    movss [r9], xmm0

    ; move to next
    inc r8
    add r9, 4
    
    dec rcx
    jnz .loop_start

finish:
    ret