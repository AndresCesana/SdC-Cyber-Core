section .data
    gini dd 0                ; Variable global para guardar un valor entero temporal

global suma_1_gini          ; Hace visible la función para poder usarla desde otros archivos (por ejemplo, desde C)

section .text               ; Sección de código ejecutable

suma_1_gini:
    push ebp                ; Guarda el valor actual del registro EBP (base pointer) en la pila
    mov ebp, esp            ; Establece el nuevo marco de pila (stack frame)

    fld dword [ebp + 8]     ; Carga el argumento float (ubicado en [ebp + 8]) a la pila de la FPU (ST(0))

    fistp dword [gini]      ; Convierte el valor flotante a entero y lo guarda en la variable global "gini"
                            ; Luego saca ese valor de la pila de la FPU

    mov eax, [gini]         ; Carga el valor entero guardado en "gini" al registro EAX
    add eax, 1              ; Le suma 1 al valor de EAX
    mov [gini], eax         ; (Opcional) Guarda el nuevo valor en "gini", aunque no es necesario si solo queremos devolverlo

    mov esp, ebp            ; Restaura el puntero de pila (ESP) al valor que tenía antes de entrar a la función
    pop ebp                 ; Recupera el valor original de EBP
    ret                     ; Retorna al llamador. El resultado queda en EAX (registro estándar de retorno)

section .note.GNU-stack noalloc noexec nowrite progbits
