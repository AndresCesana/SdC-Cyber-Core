[BITS 16]                ; En modo real (16 bits)
[ORG 0x7C00]             ; Dirección de carga estándar del bootloader


start:
    
    cli                 ; 1. Desactivar interrupciones


    ; 2. Cargar la GDT
    lgdt [gdt_descriptor]


    ; 3. Activar el bit PE en CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax


    ; 4. Saltar a modo protegido
    jmp 0x08:protected_mode_start  ; salto lejano con selector de código


; —-----------------------------------------------------------
; Definimos la GDT
gdt_start:
    dq 0x0000000000000000        ; Descriptor nulo obligatorio
    dq 0x00CF9A000000FFFF        ; Descriptor de segmento de código, ejecutable
    dq 0x00CF92000000FFFF        ; Descriptor de segmento de datos, lectura/escritura
gdt_end:


gdt_descriptor:
    dw gdt_end - gdt_start - 1   ; Tamaño de la GDT - 1 byte
    dd gdt_start                 ; Dirección base de la GDT


; —-------------------------------------------------------------
; Código en modo protegido
[BITS 32] 		  ; A partir de aca estamos en modo protegido (32 bits)
protected_mode_start:
    mov ax, 0x10          ; Selector del descriptor de datos en la GDT
    mov ds, ax		   ; Actualizar segmento de datos
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax		   ; Segmento de pila


; Escribimos una P verde en pantalla para demostrar que estamos en PM
    mov dword [0xB8000], 0x2F50 
.halt:
    jmp .halt 	  	  ; Bucle infinito para evitar que el código avance


; —------------------------------------------------------------------------
; Relleno hasta 512 bytes
times 510 - ($ - $$) db 0	 ; Rellenar con ceros hasta 510 bytes
dw 0xAA55			 ; Firma de sector de arranque (boot signature)
