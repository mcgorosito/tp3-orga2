; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"

global start
extern GDT_DESC
extern IDT_DESC
extern idt_init
extern pic_reset
extern pic_enable
extern mmu_initKernelDir
extern mmu_mapPage
extern mmu_initTaskDir
extern screen_printNumeroLibreta
extern tss_init_initial
extern tss_init_idle
extern tss_init
extern sched_init
extern game_init

;; Defines
%define EBP_INIT 0x00000
%define ESP_INIT 0x27000
%define CS_RING_0 (0x8 << 3)
%define DS_RING_0 (0xA << 3)
%define VM_RING_0 (0xC << 3)
%define INITIAL_TASK_SEL (0xD << 3)
%define IDLE_TASK_SEL (0xE << 3)
%define SCREEN_INIT 0xb8000
%define LIGHT_RED 0xcc00cc00
%define LIGHT_BLUE 0x99009900
%define WHITE_ON_PINK 0xdd00dd00
%define BLACK 0x00000000
%define PAGING_FLAG 0x80000000

;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    print_text_rm start_rm_msg, start_rm_len, 0x07, 0, 0
    
    ; Habilitar A20 (activa la posibilidad de direccionar arriba de la address 20 (del primer MB))
    call A20_disable
    call A20_check
    call A20_enable
    call A20_check
    
    ; Cargar la GDT
    lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0
    mov eax, CR0
    or eax, 0x1
    mov cr0, eax

    jmp CS_RING_0:modo_protegido

BITS 32
modo_protegido:

    ; Establecer selectores de segmentos
    mov ax, DS_RING_0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Establecer la base de la pila
    mov ebp, EBP_INIT
    mov esp, ESP_INIT

    ; Imprimir mensaje de bienvenida
    print_text_pm start_pm_msg, start_pm_len, 0x07, 0, 0

    ; Inicializar pantalla
    mov ax, VM_RING_0
    mov es, ax

    call pintar_pantalla
    
    ; Inicializar el manejador de memoria y el directorio de paginas
    call mmu_initKernelDir

    ; Cargar directorio de paginas
    mov cr3, eax

    ; Habilitar paginacion
    mov eax, cr0
    or eax, PAGING_FLAG
    mov cr0, eax

    call screen_printNumeroLibreta

    ; Inicializar tss
    call tss_init_initial

    ; Inicializar tss de la tarea Idle
    call tss_init_idle

    ; Inicializar el scheduler (carga las otras tareas)
    call sched_init
  
    ; Iniciar el juego
    call game_init
  
    ; Inicializar la IDT
    call idt_init
    
    ; Cargar IDT
    lidt [IDT_DESC]

    ; Configurar controlador de interrupciones
    call pic_reset
    call pic_enable

    ; Cargar tarea inicial
    mov ax, INITIAL_TASK_SEL
    ltr ax

    ; Habilitar interrupciones
    sti

    ; Saltar a la primera tarea: Idle
    jmp IDLE_TASK_SEL:0

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"

;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
BITS 32
pintar_pantalla:
    xor ecx, ecx               ; 80 x 50 x 2 bytes = 8000 bytes
    mov eax, WHITE_ON_PINK        ; pintar de a 2 pixeles
    
    .pintarFondo:
        cmp ecx, 6400
        je .finPintarFondo
        mov [es:ecx], eax
        add ecx, 4
        jmp .pintarFondo

    .finPintarFondo:
        mov eax, BLACK
    
    .pintarPie:
        cmp ecx, 8000
        je .finPintarPie
        mov [es:ecx], eax
        add ecx, 4
        jmp .pintarPie

    .finPintarPie:

    ; Pintar barra jugador 1 (azul) (14x3 px)
    xor ecx, ecx                 ; contador filas (3)
    mov eax, LIGHT_BLUE          ; color jugador 1
    mov esi, 6890                ; offset fila 1: 6400 + 3*160 (3 filas) + 10 (5px*2)

    .pintarBarraJugadorUno:
        cmp ecx, 3
        je .finPintarBarraJugadorUno
        mov [es:esi], eax              ; pintar 24 bytes (12 columnas)
        mov [es:esi+4], eax
        mov [es:esi+8], eax
        mov [es:esi+12], eax
        mov [es:esi+16], eax
        mov [es:esi+20], eax
        inc ecx
        add esi, 160                   ; avanzar una fila
        jmp .pintarBarraJugadorUno

    .finPintarBarraJugadorUno:
    xor ecx, ecx
    mov eax, LIGHT_RED                 ; color jugador 2
    mov esi, 7002                      ; offset barra jugador 2: 6400 + 3*160 + 120 (60px * 2)

    .pintarBarraJugadorDos:
        cmp ecx, 3
        je .fin
        mov [es:esi], eax              ; pintar 24 bytes (12 columnas)
        mov [es:esi+4], eax
        mov [es:esi+8], eax
        mov [es:esi+12], eax
        mov [es:esi+16], eax
        mov [es:esi+20], eax
        inc ecx
        add esi, 160                   ; avanzar una fila
        jmp .pintarBarraJugadorDos

    .fin:
        ret
