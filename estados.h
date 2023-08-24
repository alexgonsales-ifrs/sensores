/* 
 * File:   estados.h
 * Author: ifrs
 *
 * Created on 14 de Abril de 2015, 18:44
 */

#ifndef ESTADOS_H
#define	ESTADOS_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <stdint.h>
#include "versao.h"

    /* Possiveis estados da maquina */
    typedef enum {
        EST_INICIO,
        EST_MENU_PRINCIPAL,
        EST_CAPTURA_E_MOSTRA,
        EST_CAPTURA_E_GRAVA,
        EST_MOSTRA_TODOS,
        EST_MOSTRA_MAX_MIN,
        EST_LIMPAR,
        EST_MENU_CONF_TEMPO_AMOSTRA,
#ifdef _DEBUG_
        EST_ENVIAR_DADOS,
#endif
        EST_MENU_CONF_QUANT_SENSORES
    } Estados;

    void estado_maquina(uint8_t op);

#ifdef	__cplusplus
}
#endif

#endif	/* ESTADOS_H */

