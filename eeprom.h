/* 
 * File:   eeprom.h
 * Author: ti
 *
 * Created on 12 de Janeiro de 2014, 04:36
 */

#ifndef EEPROM_H
#define	EEPROM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

#define END_QTDE_SENSORES       0x00    //1 byte
#define END_TX_AMOSTRA          0x02    //2 bytes reservados (usar ler e gravar word)
#define END_AMOSTRA_MIN         0x04    //2 bytes
#define END_AMOSTRA_MAX         0x06    //2 bytes
#define END_QTDE_VALORES        0x08    //1 byte
#define END_CHAVE_INICIALIZACAO 0x0c    //1 byte
#define END_INICIO_AMOSTRAS     0x0e    //1 byte

#define QTD_MAX_AMOSTRAS        100

/**
 * Funcao para gravar dois bytes (big endian) na eeprom
 * @param end endereco dos dados
 * @param dado dado a ser gravado
 */
void eeprom_gravar_word(uint8_t end, uint16_t dado);

/**
 * Funcao que le dois bytes seguidos da eeprom
 * @param end endereco de memoria onde esta o dado a ser lido
 * @return valor de 16 bits (big endian)
 */
uint16_t eeprom_ler_word(uint8_t end);

#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */

