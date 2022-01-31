/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"

void print(const char* text, uint32_t x, uint32_t y, uint16_t attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    int32_t i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (uint8_t) text[i];
        p[y][x].a = (uint8_t) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y, uint16_t attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    uint32_t i;
    uint8_t letras[16] = "0123456789";

    for(i = 0; i < size; i++) {
        uint32_t resto  = numero % 10;
        numero = numero / 10;
        p[y][x + size - i - 1].c = letras[resto];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y, uint16_t attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    int32_t i;
    uint8_t hexa[8];
    uint8_t letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void screen_printException(uint32_t code) {
    print("Exception", 2, 2, WHITE_ON_PINK);
    print_dec(code, 2, 12, 2, WHITE_ON_PINK);
}

void screen_drawBox(uint32_t fInit,
                    uint32_t cInit,
                    uint32_t fSize,
                    uint32_t cSize,
                    uint8_t character,
                    uint8_t attr ) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    uint32_t f;
    uint32_t c;
    for (f = fInit; f < fInit+fSize; f++) {
    for (c = cInit; c < cInit+cSize; c++) {
          p[f][c].c = character;
          p[f][c].a = attr;
    }}
}

void screen_printKeyboardNumber(uint8_t scancode) {
    if (scancode > 0x01 && scancode < 0x0B) {
        print_hex(scancode-1, 1, 76, 2, WHITE_ON_PINK);
    } else if (scancode == 0x0B) {
        print_hex(0, 1, 76, 2, WHITE_ON_PINK);
    }
}

void screen_printNumeroLibreta() {
    print("293/18", 2, 2, WHITE_ON_PINK);
    print("721/18", 2, 4, WHITE_ON_PINK);
}

void screen_printPlayerScores(uint8_t score_c137, uint8_t score_d248) {
    print_dec(score_c137, 10, 6, 44, LIGHT_BLUE);
    print_dec(score_d248, 10, 62, 44, LIGHT_RED);
}

void screen_printCronenbergTable() {
    uint8_t row = 20;
    for (int i = 0; i < 10; i++) {
        print_dec(i, 2, row, 42, GREY_ON_BLACK);
        if (taskList[i+4].status == ALIVE){
            if (taskList[i+4].type == CronenbergC137) {
                print("C", row, 44, BLUE_ON_BLACK);
            } else if (taskList[i+4].type == CronenbergD248) {
                print("C", row, 44, RED_ON_BLACK);
            } else { 
                print("C", row, 44, WHITE_ON_BLACK);
            }
        } else {
            print("X", row, 44, DARK_GREY_ON_BLACK);
        }
        row += 4;
    }

    row = 20;
    for (int i = 10; i < 20; i++) {
        print_dec(i, 2, row, 46, GREY_ON_BLACK);
        if (taskList[i+4].status == ALIVE){
            if (taskList[i+4].type == CronenbergC137) {
                print("C", row, 48, BLUE_ON_BLACK);
            } else if (taskList[i+4].type == CronenbergD248) {
                print("C", row, 48, RED_ON_BLACK);
            } else { 
                print("C", row, 48, WHITE_ON_BLACK);
            }
        } else {
            print("X", row, 48, DARK_GREY_ON_BLACK);
        }
        row += 4;
    }
}

void screen_printEndOfGame()  {
    screen_drawBox(10, 20, 20, 40, 0, C_BG_LIGHT_GREY | C_FG_LIGHT_GREY);
    print("The winner is:", 25, 15, BLACK_ON_GREY);
    if (winner == 1) {  
        print("RICK C137", 35, 20, BLUE_ON_GREY);
        for (int i = 20; i < 60; i++) {
            print("*", i, 10, BLUE_ON_DARK_GREY);
            print("*", i, 29, BLUE_ON_DARK_GREY);
        }
    } else if (winner == 2) {
        print("RICK D248", 35, 20, RED_ON_GREY);
        for (int i = 20; i < 60; i++) {
            print("*", i, 10, RED_ON_DARK_GREY);
            print("*", i, 29, RED_ON_DARK_GREY);
        }
    } else {
        print("Everbody died.", 30, 20, BLACK_ON_GREY);
        print("RIP Cronenbergs :(", 30, 22, BLACK_ON_GREY);
        for (int i = 20; i < 60; i++) {
            print("*", i, 10, DARK_GREY);
            print("*", i, 29, DARK_GREY);
        }
    }
}

void screen_drawTasks() {
    for (int i = 0; i < 24; i++){
        if (taskList[i].status == ALIVE) {
            print(getTaskLetter(taskList[i].type), taskList[i].x, taskList[i].y, getTaskColor(taskList[i].type));
        } else {
            print(" ", taskList[i].x, taskList[i].y, DARK_GREY);
        }
    }
    for(int i = 0; i < 4; i++) {
        if (portalList[i].x != INACTIVE_PORTAL) {
        print("@", portalList[i].x, portalList[i].y, WHITE_ON_CYAN);
        }
    }
}

void screen_clearBoard() { 
    for(int i = 0; i < 80; i++){
        for(int j = 0; j < 40; j++) {
            print(" ", i, j, PINK);
        }
    }
}

void screen_clearClocks() {
    for (int i = 75; i < 80; i++) {
        print(" ", i, 47, BLACK);
        print(" ", i, 48, BLACK);
        print(" ", i, 49, BLACK);
    }
}

void screen_printDebugModeOn() {
    print("Debug mode on", 1, 48, GREY_ON_BLACK);
}

void screen_clearDebugMode() {
    print("             ", 1, 48, BLACK);
}

void screen_printDebugBox() {
    screen_drawBox(0, 20, 40, 40, 0, C_BG_BLACK);
    print("             DEBUG MODE             ", 22, 2, WHITE_ON_RED);
    screen_drawBox(4, 22, 34, 36, 20, C_BG_LIGHT_GREY | C_FG_LIGHT_GREY);
    
    print("Exception", 24, 6, WHITE_ON_GREY);
    print_dec(debugInfo.exceptionCode, 2, 34, 6, WHITE_ON_GREY);
    print("Error code", 40, 6, WHITE_ON_GREY);
    print_hex(debugInfo.errorCode, 2, 51, 6, WHITE_ON_GREY);
        
    print("eax", 24, 8, WHITE_ON_GREY);
    print_hex(debugInfo.eax, 8, 28, 8, BLACK_ON_GREY);
    print("ebx", 24, 10, WHITE_ON_GREY);
    print_hex(debugInfo.ebx, 8, 28, 10, BLACK_ON_GREY);
    print("ecx", 24, 12, WHITE_ON_GREY);
    print_hex(debugInfo.ecx, 8, 28, 12, BLACK_ON_GREY);
    print("edx", 24, 14, WHITE_ON_GREY);
    print_hex(debugInfo.edx, 8, 28, 14, BLACK_ON_GREY);
    print("esi", 24, 16, WHITE_ON_GREY);
    print_hex(debugInfo.esi, 8, 28, 16, BLACK_ON_GREY);
    print("edi", 24, 18, WHITE_ON_GREY);
    print_hex(debugInfo.edi, 8, 28, 18, BLACK_ON_GREY);
    print("ebp", 24, 20, WHITE_ON_GREY);
    print_hex(debugInfo.ebp, 8, 28, 20, BLACK_ON_GREY);
    print("esp", 24, 22, WHITE_ON_GREY);
    print_hex(debugInfo.esp, 8, 28, 22, BLACK_ON_GREY);
    print("eip", 24, 24, WHITE_ON_GREY);
    print_hex(debugInfo.eip, 8, 28, 24, BLACK_ON_GREY);
        
    print("cr0", 24, 28, WHITE_ON_GREY);
    print_hex(debugInfo.cr0, 8, 28, 28, BLACK_ON_GREY);
    print("cr2", 24, 30, WHITE_ON_GREY);
    print_hex(debugInfo.cr2, 8, 28, 30, BLACK_ON_GREY);
    print("cr3", 24, 32, WHITE_ON_GREY);
    print_hex(debugInfo.cr3, 8, 28, 32, BLACK_ON_GREY);
    print("cr4", 24, 34, WHITE_ON_GREY);
    print_hex(debugInfo.cr4, 8, 28, 34, BLACK_ON_GREY);
        
    print("cs", 40, 8, WHITE_ON_GREY);
    print_hex(debugInfo.cs, 4, 44, 8, BLACK_ON_GREY);
    print("ds", 40, 10, WHITE_ON_GREY);
    print_hex(debugInfo.ds, 4, 44, 10, BLACK_ON_GREY);
    print("es", 40, 12, WHITE_ON_GREY);
    print_hex(debugInfo.es, 4, 44, 12, BLACK_ON_GREY);
    print("fs", 40, 14, WHITE_ON_GREY);
    print_hex(debugInfo.fs, 4, 44, 14, BLACK_ON_GREY);
    print("gs", 40, 16, WHITE_ON_GREY);
    print_hex(debugInfo.gs, 4, 44, 16, BLACK_ON_GREY);
    print("ss", 40, 18, WHITE_ON_GREY);
    print_hex(debugInfo.ss, 4, 44, 18, BLACK_ON_GREY);

    print("eflags", 40, 21, WHITE_ON_GREY);
    print_hex(debugInfo.eflags, 8, 47, 21, BLACK_ON_GREY);
        
    print("stack", 40, 24, WHITE_ON_GREY);
    print_hex(debugInfo.stack1, 8, 40, 26, BLACK_ON_GREY);
    print_hex(debugInfo.stack2, 8, 40, 28, BLACK_ON_GREY);
    print_hex(debugInfo.stack3, 8, 40, 30, BLACK_ON_GREY);
    print_hex(debugInfo.stack4, 8, 40, 32, BLACK_ON_GREY);
    print_hex(debugInfo.stack5, 8, 40, 34, BLACK_ON_GREY);
}

/* Funciones auxiliares */
char* getTaskLetter(e_taskType_t taskType) {
    if (taskType == RickC137 || taskType == RickD248) {
        return "R";
    } else if (taskType == MortyC137 || taskType == MortyD248) {
        return "M";
    } else {
        return "C";
    }
}

uint16_t getTaskColor(e_taskType_t taskType) {
    if (taskType == RickC137 || taskType == MortyC137 || taskType == CronenbergC137) {
        return LIGHT_BLUE;
    } else if (taskType == RickD248 || taskType == MortyD248 || taskType == CronenbergD248) {
        return LIGHT_RED;
    } else {
        return WHITE_ON_GREEN;
    }
}