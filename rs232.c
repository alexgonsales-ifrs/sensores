#include <stdint.h>
#include <xc.h>

#include "eeprom.h"
#include "quant_sensores.h"
#include "xtal.h"

void rs232_init(void) {
#ifdef _PIC16F886_H_
    BRG16 = 0;
#endif
    TRISC6 = 0; //tx = saida
    TRISC7 = 1; //rx = entrada
    TXEN = 1;
    SYNC = 0;
    SPEN = 1;
    CREN = 1;
    PIE1bits.RCIE = 1;
    INTCONbits.PEIE = 1;   
    BRGH = 0;
    SPBRG = 25; //2400 BPS
    TX9 = 0;
    PIR1bits.RCIF = 0;
}

uint8_t rs232_transmite(void) {
    uint8_t qtd_val, x;
    //uint16_t qtd_sens;
    uint8_t qtd_sens, qtd_bytes;
    uint8_t i = 0;

    qtd_val = eeprom_read(END_QTDE_VALORES);
    qtd_bytes = qtd_val * 2;

    //solucao anterior:
    /*
    qtd_sens = (uint8_t) eeprom_read(END_QTDE_SENSORES);
    TXREG = qtd_sens;
    qtd_sens = qtd_sens >> 8;
    __delay_us(100);
    TXREG = qtd_sens;
    __delay_us(100);
    */

    //o java trabalha com inteiros de 16 bits com sinal (little endian):
    qtd_sens = eeprom_read(END_QTDE_SENSORES);
    TXREG = menu_quant_sensores[qtd_sens].i_quant_sensores;
    //2400 bauds = (0,000416667 s) * (10 bits em cada transmissao serial) = 4.16 ms
    __delay_ms(5);
    TXREG = 0;
    __delay_ms(5);
    
    for (; i < qtd_bytes; i = i + 2) {
        x = eeprom_read(END_INICIO_AMOSTRAS + i + 1);
        TXREG = x;
        __delay_ms(5);

        x = eeprom_read(END_INICIO_AMOSTRAS + i);
        TXREG = x;
        __delay_ms(5);
    }
    return i;
}