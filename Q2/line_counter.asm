; ============================================================
; Program: Count Lines in a Text File (names.txt)
; Platform: Linux (x86 32-bit)
; Assembler: NASM
; To Compile: nasm -f elf32 line_counter.asm -o line_counter.o
; To Link:    ld -m elf_i386 line_counter.o -o line_counter
; Run:        ./line_counter
; ============================================================

section .data
    filename    db "names.txt", 0         ; File to open
    bufsize     equ 4096                  ; Buffer size
    buffer      times bufsize db 0        ; Input buffer
    newline     db 10                     ; ASCII for '\n'
    out_msg     db "Line count: ", 0
    out_msg_len equ $ - out_msg

section .bss
    line_count  resd 1                    ; Holds final line count
    count_buf   resb 12                   ; Buffer for int-to-string

section .text
    global _start

_start:

    ; Open file: int open(const char *pathname, int flags, mode_t mode);
    ; syscall number: 5
    ; flags = 0 (O_RDONLY)
    mov eax, 5              ; sys_open
    mov ebx, filename       ; file name
    mov ecx, 0              ; read-only
    int 0x80

    cmp eax, 0
    js file_error

    mov esi, eax            ; Save file descriptor in esi

    ; Read from file
    ; syscall number: 3
    mov eax, 3              ; sys_read
    mov ebx, esi            ; file descriptor
    mov ecx, buffer         ; buffer
    mov edx, bufsize        ; size
    int 0x80

    cmp eax, 0
    jle file_error          ; No data or error

    mov edi, buffer         ; EDI points to buffer
    mov ecx, eax            ; ECX = number of bytes read
    xor eax, eax            ; clear EAX (line counter)

count_loop:
    cmp byte [edi], 10      ; Check for '\n'
    jne skip

    inc eax                 ; Increment line count

skip:
    inc edi                 ; Move to next byte
    loop count_loop

    mov [line_count], eax   ; Save final line count

    ; Print message
    mov eax, 4              ; sys_write
    mov ebx, 1              ; stdout
    mov ecx, out_msg
    mov edx, out_msg_len
    int 0x80

    ; Convert line_count to string and print
    mov eax, [line_count]
    mov edi, count_buf + 11 ; point to end of buffer
    mov byte [edi], 0       ; null terminator
    dec edi

convert_loop:
    xor edx, edx
    mov ebx, 10
    div ebx                 ; EAX / 10 -> quotient in EAX, remainder in EDX

    add dl, '0'
    mov [edi], dl
    dec edi
    test eax, eax
    jnz convert_loop

    inc edi

    ; Print result string
    mov eax, 4
    mov ebx, 1
    mov ecx, edi
    mov edx, count_buf + 11
    sub edx, edi
    int 0x80

    ; Exit program
    mov eax, 1
    xor ebx, ebx
    int 0x80

file_error:
    ; Print error message and exit
    mov eax, 4
    mov ebx, 1
    mov ecx, err_msg
    mov edx, err_msg_len
    int 0x80

    mov eax, 1
    mov ebx, 1
    int 0x80

section .data
    err_msg     db "File error or empty.\n", 0
    err_msg_len equ $ - err_msg
