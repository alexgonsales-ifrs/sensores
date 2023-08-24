/* 
 * File:   botoes.h
 * Author: ti
 *
 * Created on 30 de Dezembro de 2013, 11:33
 */

#ifndef BOTOES_H
#define	BOTOES_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <stdint.h>
#include "versao.h"

    //valores de retorno dos botoes
    //nao usar zero, pois a funcao btns_testa retorna 0 por default
#define P_START     1
#define P_STOP      2
#define P_UP        3
#define P_DOWN      4
    /****************************************/
#ifdef _MODULO_NOVO_
#define BT_START_SEL           RB4
#define BT_START_SEL_TRIS      TRISB4

#define BT_STOP_ESC            RB5
#define BT_STOP_ESC_TRIS       TRISB5

#define BT_UP                  RB6
#define BT_UP_TRIS             TRISB6

#define BT_DOWN                RB7
#define BT_DOWN_TRIS           TRISB7
#else #ifdef _MODULO_ANTIGO_
#define BT_START_SEL           RC3
#define BT_START_SEL_TRIS      TRISC3

#define BT_STOP_ESC            RC2
#define BT_STOP_ESC_TRIS       TRISC2

#define BT_UP                  RC1
#define BT_UP_TRIS             TRISC1

#define BT_DOWN                RC0
#define BT_DOWN_TRIS           TRISC0
#endif

    /**
     * Funcao que inicializa a porta B, configurando para ativar as interrupcoes
     * conforme os pinos onde instalamos os botoes.
     */
    void btns_init(void);

    /**
     * Funcao que faz o debouncing do sinal dos botoes.
     * @return botao pressionado
     */
    uint8_t btns_testa(void);

#ifdef	__cplusplus
}
#endif

#endif	/* BOTOES_H */

