#include <stdint.h>
#include <xc.h>
#include "lcd.h"
#include "versao.h"
#include "xtal.h"

void lcd_write(uint8_t c);

/**
 * Envia o comando para o display separando cada bit
 * @param c comando a ser enviado
 */
void lcd_write(uint8_t c) {
    if (c & 0b00000001) {
        LCD_D4 = 1;
    } else {
        LCD_D4 = 0;
    }
    if (c & 0b00000010) {
        LCD_D5 = 1;
    } else {
        LCD_D5 = 0;
    }
    if (c & 0b00000100) {
        LCD_D6 = 1;
    } else {
        LCD_D6 = 0;
    }
    if (c & 0b00001000) {
        LCD_D7 = 1;
    } else {
        LCD_D7 = 0;
    }
    __delay_us(40);
    LCD_E = 1;
    LCD_E = 0;
}

void lcd_clear(void) {
    LCD_RS = 0;
    lcd_write(0);
    lcd_write(1);
    __delay_ms(2);
}

void lcd_goto(uint8_t y, uint8_t x) {
    LCD_RS = 0;
    switch (y) {
        case 1:
            lcd_write(8);
            break;
        case 2:
            lcd_write(0xc);
            break;
    }
    lcd_write(x);
    __delay_us(40);
}

void lcd_puts(const char *str) {
    uint8_t cmd;
    LCD_RS = 1;

    while (*str) {
        cmd = *str >> 4;
#ifdef __DEBUG
        while (!TXIF) {
            continue;
        }
        TXREG = cmd;
#else
        lcd_write(cmd);
        __delay_us(40);
#endif
#ifdef __DEBUG
        while (!TXIF) {
            continue;
        }
        TXREG = *str;
#else
        lcd_write(*str);
        __delay_us(40);
#endif
        str++;
    }
#ifdef __DEBUG
    while (!TXIF) {
        continue;
    }
    TXREG = '\n';
#endif
}

void lcd_init(void) {
    LCD_RS_TRIS = LCD_E_TRIS = LCD_D4_TRIS = LCD_D5_TRIS = LCD_D6_TRIS = LCD_D7_TRIS = 0;
    LCD_RS = LCD_E = LCD_D4 = LCD_D5 = LCD_D6 = LCD_D7 = 0; // display, 0 = saida
    lcd_write(2);
    lcd_write(8);
    lcd_write(0);
    lcd_write(0);
    lcd_write(0xc);
    lcd_write(0);
    lcd_write(6);
}