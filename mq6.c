#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <xc.h>

#include "adcon.h"
#include "lcd.h"
#include "mq6.h"
#include "versao.h"
#include "xtal.h"

//Resistance of sensor(Rs): Rs=(Vc/VRL-1)×RL

/**
 * Rotina de calibracao do sensor mq6.
 * @return a resistencia R0 de calibracao
 */
uint16_t mq6_calibrar(void) {
    uint16_t vrl, ant;
    vrl = ant = 0;
    //Descobrir R0
    //do {
    //    ant = vrl;
        for (uint8_t i = 0; i < 90; i++) {
            __delay_ms(1000);
        }
        vrl = adcon_ler_canal(0);
    //} while (ant != vrl);

    return vrl;
}

/**
 * Mostra no display o valor t_int convertido para concentracao em ppm.
 * @param t_int valor a ser mostrado
 * @param i posicao no vetor de coordenadas do display
 */
void mq_mostra(uint16_t t_int, uint8_t i) {
    //const double ninv = 1/-0.4306765581;
    //const double ninv = -2.32192809474;
    //const double k = 19.5897852088;
    char temp_str[16] = {0}; //precisa ter tamanho declarado
    char temp_str2[16] = {0};
    //char *temp_str;
    uint16_t ppm;
    double yk;

    /*formula: (y/k)^(1/n)=x */
    yk = (mq_gl_r0 / (t_int * K));
    ppm = (uint16_t)pow(yk, INV_N);
    /*
    sprintf(temp_str, "ppm=%u", pow(yk, ninv)); //resultado errado
     */
    //sprintf(temp_str, "ppm=%u", ppm);
    lcd_clear();
    sprintf(temp_str, "ppm=%u", t_int);
    lcd_goto(LCD_POSICAO[i].y, LCD_POSICAO[i].x); 
    lcd_puts(temp_str);
    /*
    sprintf(temp_str2,"r0=%d rs=%d", mq_gl_r0, t_int);
    lcd_goto(2, 0);
    lcd_puts(temp_str2);
     */
}
