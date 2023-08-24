#include <xc.h>
#include <stdint.h>

#include "adcon.h"
#include "botoes.h"
#include "eeprom.h"
#include "rs232.h"
#include "tempo_amostra.h"
#include "versao.h"

/**
 * Funcao instalada no vetor de interrupcoes
 */
void interrupt handler() {
#ifdef _MODULO_NOVO_
    uint8_t option;
#endif
    static uint16_t count_t0 = 0;
//    static uint8_t nLeituras = 0;
//    static uint16_t acc = 0;
    //static uint16_t count_t2 = 0;
    uint16_t tempo_amostra, pos_taxa;

    //uint8_t quant_val;


    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    //Ver com Tiago ou Felipe por que comentaram este código.
    //Nao precisa desabilitar a interrupcao Global???

    ///INTCONbits.GIE = 0; //desabilita interrupcao global

    if (PIE1bits.RCIE) {
        if (PIR1bits.RCIF) {
            if (RCREG == 0x41) { //letra 'A'
            //if (RCREG == 'A') { //letra 'A'
                //lcd_clear();
                //static int clr = 0;
                //if (clr == 0) {
                //    lcd_clear();
                //    clr = 1;
                //}

                //char tmp[];
                //sprintf(tmp, "%c bytes transmitidos", rs232_transmite());
                //lcd_puts(tmp);
                rs232_transmite();

                //RCIF e zerada quando se le o RCREG:
                //PIR1bits.RCIF = 0;
            }
        }
    }

    if (INTCONbits.T0IE) {
        if (INTCONbits.T0IF) {
            //            pos_taxa = eeprom_read(END_TX_AMOSTRA);
            //tempo_amostra = menu_tempo_amostra[gl_item_menu_config_amostra].ul_tempo_amostra;
            pos_taxa = eeprom_ler_word(END_TX_AMOSTRA);
            //pos_taxa = eeprom_read(END_TX_AMOSTRA);
            tempo_amostra = menu_tempo_amostra[pos_taxa].ul_tempo_amostra;

            /*
            if(nLeituras <= 20) {
                acc += adcon_ler_canal(0);
                nLeituras++;
            }
             */

            if (count_t0 != tempo_amostra) {
                count_t0++;
                /*
                quant_sens = eeprom_read(END_QTDE_SENSORES);
                if (quant_sens >= 4) {
                    if (count_t0 == (tempo_amostra / 2)) {
                        adcon_monitora(4, quant_sens);
                    }
                }
                 */
            } else {
                adcon_captura_grava();
                /*
                char temp_str[9] = {0};
                uint8_t media = acc / 20;
                div_t media_div;

                media_div = div(media, 10);
                sprintf(temp_str, "%d=%d.%d", 1, media_div.quot, media_div.rem);
                lcd_goto(LCD_POSICAO[0].y, LCD_POSICAO[0].x);
                lcd_puts(temp_str);

                nLeituras = 0;
                acc = 0;
                 */

                count_t0 = 0;
            }
            TMR0 = 39; //para dar overflow antes de 256 ints
            INTCONbits.T0IF = 0;
        }
    }
#ifdef _MODULO_NOVO_
    //if (INTCONbits.RBIE) {
    if (RBIE) {
        //if (INTCONbits.RBIF) {
        if (RBIF) {
            ///guia do xc8 especifica para nao reabilitar interrupcoes dentro de handlers:
            ///INTCONbits.RBIE = 0;

            option = btns_testa();

            if (option != 0) {
                estado_maquina(option);
            }

            //INTCONbits.RBIF = 0;
            RBIF = 0;

            //guia do xc8 especifica para nao reabilitar interrupcoes dentro de handlers:
            //INTCONbits.RBIE = 1;
        }
    }
#endif
    
    /**
     * INTCONbits.GIE = 1; //habilita interrupcao global\n
     * Nao fazer isso pois a instrucao RETFIE ja faz isso.
     * Item 5.9.4, pg. 192 User Guide XC8\n
     * Never re-enable interrupts inside the interrupt function itself. 
     * Interrupts are automatically re-enabled by hardware on execution of the
     * RETFIE instruction. Re-enabling interrupts inside an interrupt function
     * may result in code failure.
     */
}
