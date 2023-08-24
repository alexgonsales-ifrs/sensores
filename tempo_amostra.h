/* 
 * File:   tempo_amostra.h
 * Author: ifrs
 *
 * Created on 16 de Abril de 2015, 16:47
 */

#ifndef TEMPO_AMOSTRA_H
#define	TEMPO_AMOSTRA_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "global.h"

//#define TAM_MENU_TEMPO_AMOSTRA   7 // tamanho do menu taxa de amostragem
    
    typedef struct {
        const char *str_tempo_amostra;
        const uint16_t ul_tempo_amostra;
    } menu_amostra;

    const menu_amostra menu_tempo_amostra[] = {
  //    const menu_amostra menu_tempo_amostra[TAM_MENU_TEMPO_AMOSTRA] = {
        //"0.1 seg", TEMPO_01_SEGUNDO,
        //"0.5 seg", TEMPO_05_SEGUNDO,
        "1 seg", TEMPO_1_SEGUNDO,
        "10 seg", TEMPO_10_SEGUNDOS,
        "30 seg", TEMPO_30_SEGUNDOS,
        "1 min", TEMPO_1_MINUTO,
        "10 min", TEMPO_10_MINUTOS,
        "30 min", TEMPO_30_MINUTOS,
        "1 hora", TEMPO_1_HORA
    };


#ifdef	__cplusplus
}
#endif

#endif	/* TEMPO_AMOSTRA_H */

