#include <stdint.h>
#include <stdlib.h>
#include <xc.h>
#include <stdio.h>

#include "adcon.h"
#include "eeprom.h"
#include "global.h"
#include "lcd.h"
#include "quant_sensores.h"
#include "xtal.h"
#include "versao.h"

#ifdef _MQ_
#include "mq6.h"
#endif

void adcon_captura_grava(void) {
    uint16_t t_int;
    uint16_t maior, menor;
    uint8_t qtd_val, qtd_sens;
    uint8_t posicao;

    //vetor que guarda a quantidade de sensores inicia em 1:
    /* Leituras ja foram feitas na transicao de menu inicial para captura e grava
    gl_item_menu_config_sensores = eeprom_read(END_QTDE_SENSORES);
    qtd_sens = menu_quant_sensores[gl_item_menu_config_sensores].i_quant_sensores;
     */
    posicao = eeprom_read(END_QTDE_SENSORES);
    qtd_sens = menu_quant_sensores[posicao].i_quant_sensores;

    qtd_val = eeprom_read(END_QTDE_VALORES);
    maior = eeprom_ler_word(END_AMOSTRA_MAX);
    menor = eeprom_ler_word(END_AMOSTRA_MIN);

    //<<<<<<<<<<<<<<
    //testar aqui se vai dar para gravar todos os valores

    lcd_clear();

    if (gl_estado == EST_CAPTURA_E_MOSTRA) {
        for (uint8_t i = 0; i < qtd_sens; i++) {
            uint16_t x = adcon_ler_canal(i);
            adcon_mostra(x, i);
            //adcon_mostra(adcon_ler_canal(i), i);
        }
    } else {
        if (gl_estado == EST_CAPTURA_E_GRAVA) {
            for (uint8_t i = 0; i < qtd_sens; i++) {
                //if ( (qtd_val + i) >= QTD_MAX_AMOSTRAS) {
                if (qtd_val >= QTD_MAX_AMOSTRAS) {
                    INTCONbits.T0IE = 0;
                    lcd_clear();
                    lcd_puts("Memoria Cheia");
                    break;
                }//if ((qtd_val + i) >= QTD_MAX_AMOSTRAS)
                else {
                    t_int = adcon_ler_canal(i);
                    if (t_int < menor) {
                        eeprom_gravar_word(END_AMOSTRA_MIN, t_int);
                    }
                    if (t_int > maior) {
                        eeprom_gravar_word(END_AMOSTRA_MAX, t_int);
                    }
                    eeprom_gravar_word(END_INICIO_AMOSTRAS + (qtd_val * 2), t_int);
                    //eeprom_gravar_word(END_INICIO_AMOSTRAS + qtd_val + (i * 2), t_int);
                    qtd_val++;
                    eeprom_write(END_QTDE_VALORES, qtd_val);
                    //eeprom_write(END_QTDE_VALORES, qtd_val + 1);
                    adcon_mostra(t_int, i);
                    //eeprom_read(END_QTDE_VALORES);

                } //else
            } // for (uint8_t i = 0; i < qtd_sens; i++)
        } // if(gl_estado == EST_CAPTURA_E_GRAVA)
    } //else
} //void adcon_captura_grava(void)

uint16_t adcon_ler_canal(uint8_t cnl) {
    /*
    ADCON0bits.CHS = canais[cnl]; // selecao de canal
    //tempo necessario (pior caso) apos troca de canal:
    __delay_us(20);
    ADCON0bits.GO_nDONE = 1;
    while (ADCON0bits.GO_nDONE); // espera terminar conversao

    return (ADRESH << 8) +ADRESL;
     */
    //const int n_valores = 30;

    //uint16_t acc = 0;
    uint32_t acc = 0;
    ADCON0bits.CHS = canais[cnl]; // selecao de canal
    __delay_us(20);
    //tempo necessario (pior caso) apos troca de canal
    for (uint16_t i = 0; i < ADCON_MEDIA; i++) {

        ADCON0bits.GO_nDONE = 1;
        while (ADCON0bits.GO_nDONE); // espera terminar conversao
        acc += ((ADRESH << 8) + ADRESL); //10bits
    }

    //acc (11 bits) para 10:
    return acc / ADCON_MEDIA;
    //return (ADRESH << 8) +ADRESL;
}

void adcon_mostra(uint16_t t_int, uint8_t i) {
    //Desta maneira deu problemas no display:
    //char temp_str2[8] = {0, 0, 0, 0, 0, 0, 0, 0 };

    char temp_str[9] = {0};
#ifdef _LM35_
    div_t temp_div;

    temp_div = div(t_int, 10);
    //evitar funcoes passadas como parametros:
    //sprintf(temp_str, "%d=%d.%d", i + 1, t_int / 10, t_int % 10);
    //sprintf(temp_str, "%d=%d.%d", i + 1, temp_div.quot, temp_div.rem);
    sprintf(temp_str, "%d=%d.%d", menu_quant_sensores[i].i_quant_sensores, temp_div.quot, temp_div.rem);
#endif
#ifdef _MQ_
    //uint16_t ppm = potencia(t_int);
    //uint16_t ppm;
    //double yk;
    //yk = (mq_gl_r0 / (t_int * K));
    ppm = (uint16_t) pow(yk, INV_N);
    //mq_mostra(ppm, 0);
    //mq_mostra(t_int, 0);
    //lcd_clear();
    sprintf(temp_str, "ppm=%u", ppm);
#endif
    lcd_goto(LCD_POSICAO[i].y, LCD_POSICAO[i].x);
    lcd_puts(temp_str);
}

void adcon_init(void) {
    TRISA = 0xff;
#ifdef _PIC16F886_H_
    ANSEL = 0b00011111; /// configura porta como analogica
    //(AN7:5) nao implementadas no PIC16F886
    ADCON1bits.VCFG1 = 0; // Referencia negativo
    // 1 = pino Vref- ; 0 = Vss
    ADCON1bits.VCFG0 = 1; // Referencia positivo
    // 1 = pino Vref+ ; 0 = Vdd
#endif
#ifdef _PIC16F876A_H_
    ADCON1bits.PCFG = 0b0001; //< configurado para q o pino seja a referencia de tensao
#endif
    ADCON0bits.ADCS = 1; // frequencia de conversao: FOSC/8 (recomendado para 4MHz)
    ADCON1bits.ADFM = 1; // resultado justificado a direita
    ADCON0bits.ADON = 1; //< liga conversor A/D
}
