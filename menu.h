/* 
 * File:   menu.h
 * Author: ti
 *
 * Created on 30 de Dezembro de 2013, 23:59
 */

#ifndef MENU_H
#define	MENU_H

#ifdef	__cplusplus
extern "C" {
#endif

//#include <stdint.h>
#include "global.h"
#include "versao.h" //versao deve ser incluido por último
    
#ifdef _DEBUG_
#define TAM_MENU_INICIAL  8 // tamanho do menu inicial
#else
#define TAM_MENU_INICIAL  7 // tamanho do menu inicial
#endif

#define TAM_MENU_QUANT_SENSORES   4 // tamanho do menu quantidade de sensores

    //const char *menu_inicial[TAM_MENU_INICIAL] = {
    const char *menu_inicial[] = {
        "Monitora", //0
        "Monitora/Grava", //1
        "Ver Todos", //2
        "Max/Min", //3
        "Limpar", //4
        "Tempo Amostra", //5
        "Quant Sensores" //6
#ifdef _DEBUG_
        , "Enviar Dados" //7
#endif

    };

#ifdef	__cplusplus
}
#endif

#endif	/* MENU_H */