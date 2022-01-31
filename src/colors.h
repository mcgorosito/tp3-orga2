#ifndef __COLORS_H__
#define __COLORS_H__

/* Definicion de colores */
/* -------------------------------------------------------------------------- */
#define C_FG_BLACK              (0x0)
#define C_FG_BLUE               (0x1)
#define C_FG_GREEN              (0x2)
#define C_FG_CYAN               (0x3)
#define C_FG_RED                (0x4)
#define C_FG_MAGENTA            (0x5)
#define C_FG_BROWN              (0x6)
#define C_FG_LIGHT_GREY         (0x7)
#define C_FG_DARK_GREY          (0x8)
#define C_FG_LIGHT_BLUE         (0x9)
#define C_FG_LIGHT_GREEN        (0xA)
#define C_FG_LIGHT_CYAN         (0xB)
#define C_FG_LIGHT_RED          (0xC)
#define C_FG_LIGHT_MAGENTA      (0xD)
#define C_FG_LIGHT_BROWN        (0xE)
#define C_FG_WHITE              (0xF)

#define C_BG_BLACK              (0x0 << 4)
#define C_BG_BLUE               (0x1 << 4)
#define C_BG_GREEN              (0x2 << 4)
#define C_BG_CYAN               (0x3 << 4)
#define C_BG_RED                (0x4 << 4)
#define C_BG_MAGENTA            (0x5 << 4)
#define C_BG_BROWN              (0x6 << 4)
#define C_BG_LIGHT_GREY         (0x7 << 4)

/* Combinaciones armadas */
#define PINK                    (0x5555)
#define BLACK                   (0x0000)
#define WHITE_ON_PINK 			(0x5F5F)
#define LIGHT_BLUE 				(0x1F1F)
#define LIGHT_RED				(0x4F4F)
#define WHITE_ON_BLACK		    (0x0F0F)
#define WHITE_ON_GREEN			(0x2F2F)
#define DARK_GREY 				(0x0F0F)
#define WHITE_ON_CYAN    		(0x3F3F)
#define BLUE_ON_BLACK           (0x0101)
#define RED_ON_BLACK            (0x0404)
#define GREY_ON_BLACK           (0x0707)
#define DARK_GREY_ON_BLACK      (0x0808)
#define RED_ON_DARK_GREY        (0x0404)
#define BLUE_ON_DARK_GREY       (0x0101)
#define RED_ON_GREY             (0x7474)
#define BLUE_ON_GREY            (0x7171)
#define WHITE_ON_GREY           (0x7F7F)
#define BLACK_ON_GREY           (0x7070)
#define DARK_GREY_ON_GREY       (0x7070)
#define WHITE_ON_RED            (0x4F4F)

#define C_BLINK                 (0x8 << 4)

#endif /* !__COLORS_H__ */
