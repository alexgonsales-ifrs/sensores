/* 
 * File:   global.h
 * Author: ifrs
 *
 * Created on 2 de Abril de 2014, 18:38
 */

#ifndef GLOBAL_H
#define	GLOBAL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "estados.h"

    // Valores de selecao do tempo de amostragem
    //#define     TEMPO_01_SEGUNDO    18/10
    //#define     TEMPO_05_SEGUNDO    9
#define     TEMPO_1_SEGUNDO     18
#define     TEMPO_10_SEGUNDOS   180
#define     TEMPO_30_SEGUNDOS   540
#define     TEMPO_1_MINUTO      1080
#define     TEMPO_10_MINUTOS    10800
#define     TEMPO_30_MINUTOS    32400
#define     TEMPO_1_HORA        64800

#define VALOR_MAXIMO_AMOSTRA               0xffff
#define VALOR_MINIMO_AMOSTRA               0
    // 1 SEGUNDO = (posicao 2 do vetor):
    //#define TEMPO_AMOSTRA_DEFAULT               2
    // 1 SEGUNDO = (posicao 2 do vetor):
#define TEMPO_AMOSTRA_DEFAULT               0

    // 1 SENSOR = (posicao 0 do vetor)
#define QUANT_SENSORES_DEFAULT              0
#define VALOR_CHAVE         93                  //94
    /**/

    Estados gl_estado = EST_INICIO;
    /*uint8_t gl_item_menu_principal = 0;
    uint8_t gl_item_menu_ver_todos = 0;
    uint16_t gl_item_menu_config_amostra = TEMPO_AMOSTRA_DEFAULT; //posicao 2 do vetor = 1 segundo (estao reservados 2 bytes de endereco)
    uint8_t gl_item_menu_config_sensores = QUANT_SENSORES_DEFAULT;*/

#ifdef	__cplusplus
}
#endif

#endif	/* GLOBAL_H */

