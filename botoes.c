#include <xc.h>

#include "botoes.h"
#include "versao.h"
#include "xtal.h"

uint8_t btns_testa(void) {
    //__delay_ms(100);
    __delay_ms(200);
    if(BT_START_SEL == 0) {
        return P_START;
    }
    if(BT_STOP_ESC == 0) {
        return P_STOP;
    }
    if(BT_UP == 0) {
        return P_UP;
    }
    if(BT_DOWN == 0) {
        return P_DOWN;
    }
    return 0;
}

void btns_init(void) {
    //TRISB =  botoes, entrada
    //WPUB = HABILITA PULL-UPS (PORTB)
    //IOCB = interrupcoes PORTB
    
    //PORTB = 0;
    BT_START_SEL = BT_STOP_ESC = BT_UP = BT_DOWN = 0;
    BT_START_SEL_TRIS = BT_STOP_ESC_TRIS = BT_UP_TRIS = BT_DOWN_TRIS = 1;
#ifdef _PIC16F886_H_ //portabilidade
    OPTION_REGbits.nRBPU = 0;  //HABILITA PULL-UPS (BIT ~WBPU)
    WPUB = 0xf0;    // weak pull-up portb
    
    //DESABILITA ANALoGICAS 11 a 13
    //obs: nao funciona se deixar ans12 como 0
    //ANSELH = 0b00000111;
    ANSELH = 0b00010010;
    IOCB = 0xf0;    // interrupt on change portb
#endif
#ifndef _MODULO_ANTIGO_
    INTCONbits.RBIE = 1; // ativa interrupcao PORTB
#endif
}