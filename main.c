/* 
 * File:   main.c
 * Author: ti
 *
 * Created on 25 de Dezembro de 2013, 15:57
 */

#include <xc.h>
#include <stdlib.h>

#include "adcon.h"
#include "botoes.h"
#include "eeprom.h"
#include "global.h"
#include "lcd.h"
#include "rs232.h"
#include "timer.h"
#include "versao.h"

#ifdef _MODULO_NOVO_
  #pragma config CP = OFF, BOREN = OFF, WDTE = OFF, PWRTE = ON, FOSC = XT, LVP = OFF
#endif

#ifdef _MODULO_ANTIGO_
  #pragma config CP = OFF, BOREN = OFF, WDTE = OFF, PWRTE = ON, FOSC = XT, LVP = OFF
#endif

/**
 * Funcao principal: inicializa os dispositivos do PIC, configura a interrupcao global
 * e fica em while().
 * @return EXIT_SUCCESS
 */
int main(void) {
#ifdef _MODULO_ANTIGO_
    uint8_t option;
#endif
    btns_init();
    rs232_init();
    lcd_init();

    lcd_puts("Inicio v1.50-");
#ifdef _MODULO_NOVO_
    lcd_puts("N");
#endif
#ifdef _MODULO_ANTIGO_
    lcd_puts("A");
#endif
#ifdef _DEBUG_
    lcd_puts("D");
#endif
    lcd_goto(2, 0);
    lcd_puts("12/02/2016");

    // the Power-up Timer (72 ms duration) prevents EEPROM write:
    // 140ms = tempo maximo do power-up:
    //__delay_ms(140);

    if (eeprom_read(END_CHAVE_INICIALIZACAO) != VALOR_CHAVE) {
        eeprom_write(END_QTDE_VALORES, 0);
        eeprom_write(END_QTDE_SENSORES, QUANT_SENSORES_DEFAULT);
        eeprom_gravar_word(END_TX_AMOSTRA, TEMPO_AMOSTRA_DEFAULT);
        eeprom_gravar_word(END_AMOSTRA_MIN, VALOR_MAXIMO_AMOSTRA);
        eeprom_gravar_word(END_AMOSTRA_MAX, VALOR_MINIMO_AMOSTRA);

        for (uint8_t i = 0; i < QTD_MAX_AMOSTRAS * 2; i++) {
            eeprom_write(i + END_INICIO_AMOSTRAS, 0);
        }
        //A chave de inicializacao tem que ser a ultima a ser gravada (OBRIGATORIAMENTE),
        //pois, parece que quando o hardware gravador termina de efetuar a gravacao,
        //o microcontrolador e ligado por um breve momento, enquanto ha energia eletrica
        //residual nos capacitores do gravador.
        //<<<<<<< isto ainda precisa ser verificado <<<<<<<<<<<<<<<<
        eeprom_write(END_CHAVE_INICIALIZACAO, VALOR_CHAVE);
    } else {
//        gl_item_menu_config_amostra = eeprom_ler_word(END_TX_AMOSTRA);
//        gl_item_menu_config_sensores = eeprom_read(END_QTDE_SENSORES);
    }

    adcon_init();
    
    timer0_init();

    //timer2_init();

    INTCONbits.GIE = 1; //habilita interrupcoes globais

    while (1) {
#ifdef _MODULO_ANTIGO_
        //11 niveis de stack

        option = btns_testa();

        if (option != 0) {
            //btns_processa(option);
            estado_maquina(option);
        }
#endif
    }
    return (EXIT_SUCCESS);
}

