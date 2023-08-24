/* 
 * File:   lcd.h
 * Author: ti
 *
 * Created on 25 de Dezembro de 2013, 17:18
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "versao.h"
#ifdef _MODULO_NOVO_
#define LCD_RS              RC0
#define LCD_RS_TRIS         TRISC0
#define LCD_E               RC1
#define LCD_E_TRIS          TRISC1
#define LCD_D4              RC2
#define LCD_D4_TRIS         TRISC2

#define LCD_D5              RB2
#define LCD_D5_TRIS         TRISB2
#define LCD_D6              RB1
#define LCD_D6_TRIS         TRISB1
#define LCD_D7              RB0
#define LCD_D7_TRIS         TRISB0
    /*
    #define LCD_D5              RC3
    #define LCD_D5_TRIS         TRISC3
    #define LCD_D6              RC4
    #define LCD_D6_TRIS         TRISC4
    #define LCD_D7              RC5
    #define LCD_D7_TRIS         TRISC5
     */
#else #ifdef _MODULO_ANTIGO_
#define LCD_RS              RB0
#define LCD_RS_TRIS         TRISB0
#define LCD_E               RB1
#define LCD_E_TRIS          TRISB1
#define LCD_D4              RB2
#define LCD_D4_TRIS         TRISB2
#define LCD_D5              RB3
#define LCD_D5_TRIS         TRISB3
#define LCD_D6              RB4
#define LCD_D6_TRIS         TRISB4
#define LCD_D7              RB5
#define LCD_D7_TRIS         TRISB5
#endif

    /**
     * Limpa o display e aguarda 2ms.
     */
    void lcd_clear(void);

    /**
     * Muda a posicao do cursor no display.
     * @param y numero da linha
     * @param x numero da coluna
     */
    void lcd_goto(uint8_t y, uint8_t x);

    /**
     * Rotina para inicializar o display. E chamada na funcao main().
     */
    void lcd_init(void);

    /**
     * Escreve uma string no display.
     * @param str
     */
    void lcd_puts(const char *str);

    //typedef struct t_pos {

    typedef struct {
        uint8_t y;
        uint8_t x;
    } S_pos;

    //const S_pos LCD_POSICAO[TAM_MENU_QUANT_SENSORES] = {
    const S_pos LCD_POSICAO[] = {
        1, 0,
        1, 8,
        2, 0,
        2, 8
    };

    /*versao 8 sensores
    const S_pos LCD_POSICAO[TAM_MENU_QUANT_SENSORES] = {
        1, 0,
        1, 8,
        2, 0,
        2, 8,
        1, 0,
        1, 8,
        2, 0,
        2, 8
    };
     */
#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

