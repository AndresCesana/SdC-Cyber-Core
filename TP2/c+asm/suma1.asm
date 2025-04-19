section .data
    int_sum dd 0         ; sum of ints
    count dd 0           ; to temporarily store count

section .text
    global convert_to_ints
    global average_of_ints

; -----------------------------------------------------------------------------
; void convert_to_ints(double* in, int* out, int count)
; -----------------------------------------------------------------------------
convert_to_ints:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 8]    ; in (double*)
    mov ebx, [ebp + 12]   ; out (int*)
    mov ecx, [ebp + 16]   ; count

.loop:
    cmp ecx, 0
    jle .done

    fld qword [eax]       ; load double from in[i]
    fistp dword [ebx]     ; convert to int, store in out[i]

    add eax, 8            ; move to next double
    add ebx, 4            ; move to next int
    dec ecx
    jmp .loop

.done:
    pop ebp
    ret

; -----------------------------------------------------------------------------
; int average_of_ints(double* in, int count)
; -----------------------------------------------------------------------------
average_of_ints:
    push ebp
    mov ebp, esp

    mov esi, [ebp + 8]     ; in (double*)
    mov edi, [ebp + 12]    ; count

    xor eax, eax           ; sum = 0

.avg_loop:
    cmp edi, 0
    jle .avg_done

    fld qword [esi]        ; load double
    fistp dword [int_sum]  ; convert to int
    add eax, [int_sum]     ; sum += int(in[i])

    add esi, 8             ; move to next double
    dec edi
    jmp .avg_loop

.avg_done:
    mov ecx, [ebp + 12]    ; count
    xor edx, edx
    div ecx                ; eax = sum / count (integer division)

    mov esp, ebp
    pop ebp
    ret                    ; return average (as int) in EAX

