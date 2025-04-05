global convert_to_ints
section .text

; void convert_to_ints(double* in, int* out, int count)
; RDI = in, RSI = out, EDX = count
convert_to_ints:
    ; preserve used registers
    push rbx

.loop:
    test edx, edx
    jz .done

    ; load [rdi] (double) into FPU
    fld qword [rdi]
    ; convert to int and store in [rsi]
    fistp dword [rsi]

    add rdi, 8      ; next double
    add rsi, 4      ; next int
    dec edx
    jmp .loop

.done:
    pop rbx
    ret
section .note.GNU-stack noalloc noexec nowrite progbits
