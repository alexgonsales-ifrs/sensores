/* 
 * File:   adcon.h
 * Author: ti
 *
 * Created on 5 de Janeiro de 2014, 03:38
 */

#ifndef ADCON_H
#define	ADCON_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <stdint.h>
#define ADCON_MEDIA 32

const uint8_t canais[] = {
    0,
    1,
    2,
    4
};

/*const uint8_t canais[TAM_MENU_QUANT_SENSORES] = {
    0,
    1,
    2,
    4
};
 */
/*versao 8 sensores
const uint8_t canais[TAM_MENU_QUANT_SENSORES] = {
    0,
    1,
    2,
    4,  //ra5
    12, //rb0
    10, //rb1
    8,  //rb2
    9   //rb3
};
 * */
/*
0000 = AN0
0001 = AN1
0010 = AN2
0011 = AN3
0100 = AN4
0101 = AN5
0110 = AN6
0111 = AN7
1000 = AN8
1001 = AN9
1010 = AN10
1011 = AN11
1100 = AN12
1101 = AN13
1110 = CV REF
1111 = Fixed Ref (0.6 volt fixed reference)
*/

/**
 * Funcao chamada a partir da contagem do timer. Le os valores dos sensores
 * chamando a funcao adcon_ler_canal e os grava na eeprom dependendo do estado.
 */
void adcon_captura_grava(void);

/**
 * Funcao que inicializa o conversor analogico/digital. e chamada na funcao main().
 */
void adcon_init(void);

/**
 * Funcao que le os niveis de tensao medios escolhidos na constante ADCON_MEDIA
 * @param cnl canal da porta analogica a ser lido (0 a 3)
 * @return valor medio das leituras da porta analogica
 */
uint16_t adcon_ler_canal(uint8_t cnl);

/**
 * Funcao que divide o valor t_int por 10 mostrando no display o quociente e o resto
 * @param t_int valor lido da porta analogica (0 a 1023)
 * @param i posicao no vetor de coordenadas LCD_POSICAO do display lcd
 */
void adcon_mostra(uint16_t t_int, uint8_t i);

#ifdef	__cplusplus
}
#endif

#endif	/* ADCON_H */

