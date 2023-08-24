/* 
 * File:   quant_sensores.h
 * Author: ifrs
 *
 * Created on 16 de Abril de 2015, 15:20
 */

#ifndef QUANT_SENSORES_H
#define	QUANT_SENSORES_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <stdint.h>
    /* Estruturas de menu padronizadas com uma string e o valor do item
     Gravar e ler da EEPROM o indice de posicao nos vetores*/

    typedef struct {
        const char *str_quant_sensores;
        int8_t i_quant_sensores;
    } menu_sensores;

//const Quant_sensores menu_quant_sensores[TAM_MENU_QUANT_SENSORES] = {
    const menu_sensores menu_quant_sensores[] = {
        "1 sensor", 1,
        "2 sensores", 2,
        "3 sensores", 3,
        "4 sensores", 4
    };

#ifdef	__cplusplus
}
#endif

#endif	/* QUANT_SENSORES_H */

