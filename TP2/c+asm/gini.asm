section .data
    int_sum dd 0

section .text
    global convert_to_ints
    global average_of_ints

; void convert_to_ints(double* in, int* out, int count)
convert_to_ints:
    ; rdi = in (double*)
    ; rsi = out (int*)
    ; rdx = count

.loop:
    cmp rdx, 0
    je .done

    fld qword [rdi]          ; load double
    fistp dword [rsi]        ; store as int

    add rdi, 8               ; next double
    add rsi, 4               ; next int
    dec rdx
    jmp .loop

.done:
    ret

; int average_of_ints(double* in, int count)
average_of_ints:
    ; rdi = in (double*)
    ; rsi = count

    xor rax, rax             ; sum = 0

.avg_loop:
    cmp rsi, 0
    je .avg_done

    fld qword [rdi]
    fistp dword [int_sum]
    add eax, [int_sum]

    add rdi, 8
    dec rsi
    jmp .avg_loop

.avg_done:
    test rsi, rsi            ; Check if count is zero
    jz .error                ; Handle division by zero

    xor edx, edx             ; Clear the upper 32 bits of the dividend
    mov ecx, rsi             ; Move count to ecx (divisor)
    div ecx                  ; Perform unsigned division: RAX = RAX / ECX
    ret

.error:
    mov rax, -1              ; Return -1 to indicate an error
    ret
