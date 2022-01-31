; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "print.mac"

BITS 32

sched_task_offset:    dd 0xFFFFFFFF
sched_task_selector:  dw 0xFFFF
morty_coord_x:        dd 0xFFFFFFFF
morty_coord_y:        dd 0xFFFFFFFF
usePortalGun_res:     dd 0xFFFFFFFF
debug_mode_flag:      dd 0x00000000
exception_flag:       dd 0x00000000

;; PIC
extern pic_finish1

;; Sched
extern sched_nextTask
extern sched_saveDebugInfo
extern sched_exceptionHasErrorCode

;; Game
extern game_updateScore
extern game_checkEndOfGame
extern game_whereIsMorty
extern game_iAmRick
extern game_usePortalGun
extern game_killActiveCharacter

;; Print
extern screen_printKeyboardNumber
extern screen_printException
extern screen_printEndOfGame
extern screen_clearClocks
extern screen_printDebugModeOn
extern screen_clearDebugMode
extern screen_printDebugBox
extern screen_clearBoard
extern screen_drawTasks


%define IDLE_TASK_SEL     (0xE << 3)
%define DEBUG_ON          0xFFFFFFFF
%define DEBUG_NO_SCREEN   0x55555555
%define DEBUG_OFF         0x00000000
%define EXCEPTION_SET     0xFFFFFFFF
%define EXCEPTION_CLR     0x00000000
%define END_OF_STACK      0x08002000

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
    pushad
    mov edx, [debug_mode_flag]
    cmp edx, DEBUG_ON
    jne .resumeGame

    ; Ver si la excepción tiene error code
    push %1                           ; pushear código de excepción
    call sched_exceptionHasErrorCode  ; ver si la excepción tiene error code
    add esp, 4                        ; restaurar stack a estado anterior a pasar parámetro
    cmp ax, 1                         ; chequear si el resultado es 1, es decir, si tiene error code
    je .continue                      ; si tiene, continuar

    popad                             ; si no, restaurar los registros
    push 0                            ; pushear un cero en la posición del error code
    pushad                            ; volver a poner en la pila los registros

.continue:
    ; Si no hay elementos suficientes en el stack, rellenar con ceros
    ; Usar el ESP de la pila de la interrupción para recorrerla
    mov ebx, [esp+48] ; esp de nivel 3 en ebx
    mov ecx, 5        ; contador de elementos del stack ya pusheados
    
    .pushStackInfo:
        cmp ebx, END_OF_STACK
        je .completeStack
        cmp ecx, 0
        je .completeStack
        mov eax, [ebx]
        push eax
        dec ecx
        add ebx, 4
        jmp .pushStackInfo

    .completeStack:
        cmp ecx, 0
        je .endPushStackInfo
        xor eax, eax
        push eax
        dec ecx
        jmp .completeStack

    .endPushStackInfo:
    push gs
    push fs
    push es
    push ds
    mov eax, cr4
    push eax
    mov eax, cr3
    push eax
    mov eax, cr2
    push eax
    mov eax, cr0
    push eax
    push %1
    call sched_saveDebugInfo
    add esp, 56

    mov dword [exception_flag], EXCEPTION_SET ; frenar la ejecución
    call screen_printDebugBox        

    .resumeGame:
    ; Marcar que la tarea murió y actualizar scores
    ; (con esto alcanza porque sched_nextTask busca la próxima viva)
    push %1
    call screen_printException
    add esp, 4
    call game_killActiveCharacter
    popad

    ; Saltar a idle
    mov ax, IDLE_TASK_SEL
    mov word [sched_task_selector], ax  ; Carga el selector de segmento de la tarea a la que se va a saltar
    jmp far [sched_task_offset]         ; Intercambiar tareas

    jmp $

%endmacro

;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 16
ISR 17
ISR 18
ISR 19

;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
global _isr32
_isr32:
    pushad
    call pic_finish1   ; Indica que la interrupción fue atendida

    mov edx, [exception_flag]
    cmp edx, EXCEPTION_SET
    je .fin

    call game_updateScore

    call screen_clearClocks

    call game_checkEndOfGame   ; Chequear si terminó el juego
    cmp eax, 0                 ; Si no, saltar a nextTask
    je .nextTask
    call screen_printEndOfGame
    str cx                     ; Si terminó pero ya había terminado antes (ya estaba en idle), saltar a fin
    mov ax, IDLE_TASK_SEL      ; Si no, saltar a idle
    cmp ax, cx
    je .fin

    mov word [sched_task_selector], ax  ; Carga el selector de segmento de idle    
    jmp far [sched_task_offset]         ; Saltar a idle

    .nextTask:
    call nextClock             ; Imprimir el reloj del sistema
    call sched_nextTask        ; Obtener selector de la próxima tarea a ejecutar
    shl ax, 3
    str cx                     ; Comparar con la tarea actual y saltar solo si es diferente
    cmp ax, cx
    je .fin
    mov word [sched_task_selector], ax  ; Cargar el selector de segmento de la tarea a la que se va a saltar
    jmp far [sched_task_offset]         ; Intercambiar tareas

    .fin:
    popad
    iret

;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
global _isr33
_isr33:
    pushad
    call pic_finish1            ; Indicar que la interrupción fue atendida
    in al, 0x60                 ; Capturar una tecla

    push eax
    call screen_printKeyboardNumber
    add esp, 4

    cmp al, 0x15                ; Chequear si es "y" (modo debug)
    jne .continue

    ; Toggle modo debug
    mov ecx, [exception_flag]
    cmp ecx, EXCEPTION_SET
    je .clearException

    mov edx, [debug_mode_flag]  ; Chequear flag: si es cero, activar
    cmp edx, DEBUG_ON
    je .deactivateDebugMode

    .activateDebugMode:
    ; Setear flag
    mov dword [debug_mode_flag], DEBUG_ON
    call screen_printDebugModeOn
    jmp .continue

    .clearException:
    mov dword [exception_flag], EXCEPTION_CLR
    call screen_clearBoard
    call screen_drawTasks
    jmp .continue

    .deactivateDebugMode:
    mov dword [debug_mode_flag], DEBUG_OFF
    call screen_clearDebugMode
    ; Esperar la próxima interrupción de reloj
    
    .continue:
    popad
    iret

;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
global _isr137
_isr137:
    pushad
    push edx                    ; Pasar parámetros
    push ecx
    push ebx
    push eax
    call game_usePortalGun      ; worked in eax 
    add esp, 16                 ; Restaurar la pila
    mov [usePortalGun_res], eax
    popad
    mov eax, [usePortalGun_res]
    iret

global _isr138
_isr138:
    pushad
    push eax
    call game_iAmRick
    add esp, 4
    popad
    iret

global _isr139
_isr139:
    pushad
    push morty_coord_y
    push morty_coord_x
    call game_whereIsMorty
    add esp, 8
    popad
    mov eax, [morty_coord_x]
    mov ebx, [morty_coord_y]
    iret

;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
isrNumber:           dd 0x00000000
isrClock:            db '|/-\'
nextClock:
        pushad
        inc DWORD [isrNumber]
        mov ebx, [isrNumber]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrNumber], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                print_text_pm ebx, 1, 0x0f, 48, 78
                popad
        ret
