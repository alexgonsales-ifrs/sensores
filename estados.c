#include <xc.h>
#include <stdio.h>

#include "adcon.h"
#include "botoes.h"
#include "eeprom.h"
#include "global.h"
#include "lcd.h"
#include "menu.h"
#include "quant_sensores.h"
#include "tempo_amostra.h"
#include "versao.h"
/*
#include "estados.h"
#include "rs232.h"
*/

uint8_t gl_item_menu_principal = 0;
uint8_t gl_item_menu_ver_todos = 0;
uint16_t gl_item_menu_config_amostra = TEMPO_AMOSTRA_DEFAULT; //posicao 2 do vetor = 1 segundo (estao reservados 2 bytes de endereco)
uint8_t gl_item_menu_config_sensores = QUANT_SENSORES_DEFAULT;

void estado_menu_limpar(uint8_t opt);
void estado_menu_principal(uint8_t opt);
void estado_executa_muda(Estados novo_estado);
void estado_captura_mostra(uint8_t opt);
void estado_inicial(uint8_t opt);
void estado_menu_conf_tempo_amostra(uint8_t opt);
void estado_menu_conf_quant_sensores(uint8_t opt);
void estado_enviar_dados(uint8_t opt);
void estado_ver_todos(uint8_t opt);
void estado_max_min(uint8_t opt);

/**
 * Esta funcao associa o estado atual e chama os estados correspondentes
 * @param op eh a tecla que gerou o evento para a maquina de estados.
 */
void estado_maquina(uint8_t op) {

    switch (gl_estado) {
        case EST_INICIO:
            //muda_estado(EST_MENU_PRINCIPAL);
            estado_inicial(op);
        break;//EST_INICIO

        case EST_MENU_PRINCIPAL:
            estado_menu_principal(op);
        break;//EST_MENU_PRINCIPAL:

        case EST_CAPTURA_E_MOSTRA:
            estado_captura_mostra(op);
        break; //EST_CAPTURA_E_MOSTRA

        case EST_CAPTURA_E_GRAVA:
            //usando o timer 0, a funcao somente testa o stop e para o timer:

            //    if (opt == P_STOP) {
            //        INTCONbits.T0IE = 0;
            //        muda_estado(EST_MENU_PRINCIPAL);
            //    }
            estado_captura_mostra(op);
        break; //EST_CAPTURA_E_GRAVA

        case EST_MOSTRA_TODOS:
            estado_ver_todos(op);
        break; //EST_MOSTRA_TODOS

        case EST_MOSTRA_MAX_MIN:
            estado_max_min(op);
        break; //EST_MOSTRA_MAX_MIN

        case EST_LIMPAR:
            estado_menu_limpar(op);
        break; //EST_LIMPAR

        case EST_MENU_CONF_TEMPO_AMOSTRA:
            estado_menu_conf_tempo_amostra(op);
        break; //EST_MENU_CONF_TEMPO_AMOSTRA

#ifdef _DEBUG_
        case EST_ENVIAR_DADOS:
            estado_enviar_dados(op);
        break; //EST_ENVIAR_DADOS
#endif
        case EST_MENU_CONF_QUANT_SENSORES:
            estado_menu_conf_quant_sensores(op);
        break; //EST_MENU_CONF_QUANT_SENSORES
    }
}

/**
 * a partir do estado inicial, muda para o estado menu principal
 * @param opt qualquer dos botoes
 */
void estado_inicial(uint8_t opt) {
    estado_executa_muda(EST_MENU_PRINCIPAL);
}

/**
 * A funcao aguarda um Stop para sair do menu limpar para o menu inicial
 * @param opt botao stop
 */
void estado_menu_limpar(uint8_t opt) {
    if (opt == P_STOP) {
        estado_executa_muda(EST_MENU_PRINCIPAL);
    }
}

/**
 * Funcao chamada na saida dos estados EST_CAPTURA_E_MOSTRA EST_CAPTURA_E_GRAVA:
 * @param opt stop
 */
void estado_captura_mostra(uint8_t opt) {
    if (opt == P_STOP) {
        INTCONbits.T0IE = 0;
        estado_executa_muda(EST_MENU_PRINCIPAL);
    }
}

/**
 * Funcao que controla o estado menu principal e suas transicoes
 * @param opt o botao correspondente
 */
void estado_menu_principal(uint8_t opt) {
    uint8_t qtd_val;
    uint8_t qtd_sens;
    uint16_t t_int;
    uint16_t maior, menor;
    uint8_t posicao;
    
    switch (opt) {
        case P_UP:
            if (gl_item_menu_principal > 0) {
                gl_item_menu_principal--;
                estado_executa_muda(EST_MENU_PRINCIPAL);
            }
            break;

        case P_DOWN:
            if (gl_item_menu_principal < TAM_MENU_INICIAL - 1) {
                gl_item_menu_principal++;
                estado_executa_muda(EST_MENU_PRINCIPAL);
            }
            break;

        case P_START:
            //transicoes do estado_menu_principal
            switch (gl_item_menu_principal) {
                case 0: //"Monitora"
                    //IMPLEMENTAR:
                    //Ler uma amostra.
                    //Atualizar display.
                    lcd_clear();
                    //vetor que guarda a quantidade de sensores inicia em 1:
                    //gl_item_menu_config_sensores = eeprom_read(END_QTDE_SENSORES);
                    //qtd_sens = menu_quant_sensores[gl_item_menu_config_sensores].i_quant_sensores;
                    posicao = eeprom_read(END_QTDE_SENSORES);
                    qtd_sens = menu_quant_sensores[posicao].i_quant_sensores;

                    for (uint8_t i = 0; i < qtd_sens; i++) {
                        t_int = adcon_ler_canal(i);
#ifdef _LM35_
                            adcon_mostra(t_int, i);
#endif
#ifdef _MQ_
                          mq_mostra(t_int, i);
#endif

                        //evitar funcoes passadas como parametros:
                        //adcon_mostra(adcon_ler_canal(i), i);
                    }
                    estado_executa_muda(EST_CAPTURA_E_MOSTRA);
                    break;

                case 1: //"Monitora/Grava"
                    lcd_clear();
                    //vetor que guarda a quantidade de sensores inicia em 1:
//                    gl_item_menu_config_sensores = eeprom_read(END_QTDE_SENSORES);
//                    qtd_sens = menu_quant_sensores[gl_item_menu_config_sensores].i_quant_sensores;

                    qtd_val = eeprom_read(END_QTDE_VALORES);
                    maior = eeprom_ler_word(END_AMOSTRA_MAX);
                    menor = eeprom_ler_word(END_AMOSTRA_MIN);

                    for (uint8_t i = 0; i < qtd_sens; i++) {
                        if ((qtd_val + i) >= QTD_MAX_AMOSTRAS) {
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
                            qtd_val++;
                            eeprom_write(END_QTDE_VALORES, qtd_val);
#ifdef _LM35_
                            adcon_mostra(t_int, i);
#endif
#ifdef _MQ_
                            mq_mostra(t_int, i);
#endif
                            //eeprom_read(END_QTDE_VALORES);
                        } //else
                    } //for (uint8_t i = 0; i < qtd_sens; i++)

                    estado_executa_muda(EST_CAPTURA_E_GRAVA);
                    break;

                case 2: //"Ver Todos"
                    estado_executa_muda(EST_MOSTRA_TODOS);
                    break;

                case 3: //"Max/Min"
                    estado_executa_muda(EST_MOSTRA_MAX_MIN);
                    break;

                case 4: //"Limpar"
                    estado_executa_muda(EST_LIMPAR);
                    break;

                case 5: //"Tempo Amostra"
                    gl_item_menu_config_amostra = eeprom_ler_word(END_TX_AMOSTRA);
                    estado_executa_muda(EST_MENU_CONF_TEMPO_AMOSTRA);
                    break;

                case 6: //"Quant Sensores"
                    gl_item_menu_config_sensores = eeprom_read(END_QTDE_SENSORES);
                    estado_executa_muda(EST_MENU_CONF_QUANT_SENSORES);
                    break;
#ifdef _DEBUG_
                case 7: //"Enviar Dados"
                    estado_executa_muda(EST_ENVIAR_DADOS);
                    break;
#endif
            }
            break; //P_START
    } //switch (op)
}

/**
 * Funcao que controla a navegacao por todos os valores ja gravados na eeprom
 * @param opt botao pressionado
 */
void estado_ver_todos(uint8_t opt) {
    switch (opt) {
        case P_UP:
            if (gl_item_menu_ver_todos > 0) {
                gl_item_menu_ver_todos--;
                estado_executa_muda(EST_MOSTRA_TODOS);
            }
            break;
        case P_DOWN:
            //if (gl_item_menu_ver_todos < eeprom_read(END_QTDE_VALORES) - 1) {
            //if (gl_item_menu_ver_todos < eeprom_read(END_QTDE_VALORES) + eeprom_read(END_QTDE_SENSORES)) {
            if (gl_item_menu_ver_todos < (eeprom_read(END_QTDE_VALORES) / eeprom_read(END_QTDE_SENSORES))) {
                gl_item_menu_ver_todos++;
                estado_executa_muda(EST_MOSTRA_TODOS);
            }
            break;
        case P_STOP:
            estado_executa_muda(EST_MENU_PRINCIPAL);
            break;
    }
}

/**
 * A funcao espera um stop quando esta no estado maximo/minimo
 * @param opt botao stop
 */
void estado_max_min(uint8_t opt) {
    if (opt == P_STOP) {
        estado_executa_muda(EST_MENU_PRINCIPAL);
    }
}

/**
 * Controla o item de menu que configura o tempo de amostra
 * @param opt botao pressionado
 */
void estado_menu_conf_tempo_amostra(uint8_t opt) {
    switch (opt) {
        case P_UP:
            if (gl_item_menu_config_amostra >= 1) {
                gl_item_menu_config_amostra--;
                estado_executa_muda(EST_MENU_CONF_TEMPO_AMOSTRA);
            }
            break;
        case P_DOWN:
            //if (gl_item_menu_config_amostra < TAM_MENU_TEMPO_AMOSTRA - 1) {
            if (gl_item_menu_config_amostra < ((sizeof(menu_tempo_amostra) / sizeof(menu_amostra)) - 1)) {
                gl_item_menu_config_amostra++;
                estado_executa_muda(EST_MENU_CONF_TEMPO_AMOSTRA);
            }
            break;
        case P_STOP:
            estado_executa_muda(EST_MENU_PRINCIPAL);
            break;
        case P_START:
            eeprom_gravar_word(END_TX_AMOSTRA, gl_item_menu_config_amostra);
            estado_executa_muda(EST_MENU_PRINCIPAL);
            break;
    }
}

/**
 * Aguarda um stop apos o envio dos dados pela serial para depois voltar ao menu principal
 * @param opt botao stop
 */
void estado_enviar_dados(uint8_t opt) {
    if (opt == P_STOP) {
        estado_executa_muda(EST_MENU_PRINCIPAL);
    }
}

/**
 * Controla o item de menu que configura a quantidade de sensores
 * @param opt botao pressionado
 */
void estado_menu_conf_quant_sensores(uint8_t opt) {
    switch (opt) {
        case P_UP:
            //vetor que guarda a quantidade de sensores inicia em 1:
            if (gl_item_menu_config_sensores >= 1) {
                gl_item_menu_config_sensores--;
                estado_executa_muda(EST_MENU_CONF_QUANT_SENSORES);
            }
            break;
        case P_DOWN:
            //vetor que guarda a quantidade de sensores inicia em 1:
            if (gl_item_menu_config_sensores < TAM_MENU_QUANT_SENSORES - 1) {
                gl_item_menu_config_sensores++;
                estado_executa_muda(EST_MENU_CONF_QUANT_SENSORES);
            }
            break;
        case P_STOP:
            estado_executa_muda(EST_MENU_PRINCIPAL);
            break;
        case P_START:
            eeprom_write(END_QTDE_SENSORES, gl_item_menu_config_sensores);
            estado_executa_muda(EST_MENU_PRINCIPAL);
            break;
    }
}

/**
 * executa a operacao do novo_estado e troca o estado da maquina para novo_estado
 * ou seja, gl_estado = novo_estado
 * @param novo_estado o proximo estado
 */
void estado_executa_muda(Estados novo_estado) {
    //uint16_t maior, menor, temp_int, pos_taxa;
    uint16_t maior, menor, temp_int;
    uint8_t endereco, qtd_val, qtd_sens;
    uint8_t posicao;
#ifdef _DEBUG_
    uint8_t bytes_tx;
#endif
    char tmp[17] = {0}; //warning do compilador

    //pos_taxa = eeprom_ler_word(END_TX_AMOSTRA);
    switch (novo_estado) {
        case EST_MENU_PRINCIPAL:
            lcd_clear();
            lcd_puts(menu_inicial[gl_item_menu_principal]);
            break; //EST_MENU_PRINCIPAL
        case EST_CAPTURA_E_MOSTRA:
            INTCONbits.T0IE = 1;
            break; //EST_CAPTURA_E_MOSTRA
        case EST_CAPTURA_E_GRAVA:
            INTCONbits.T0IE = 1;
            break; //EST_CAPTURA_E_GRAVA
        case EST_MOSTRA_TODOS:
            lcd_clear();
            qtd_val = eeprom_read(END_QTDE_VALORES);
            if (qtd_val == 0) {
                lcd_puts("Nenhum Dado");
            } else {
                //vetor que guarda a quantidade de sensores inicia em 1:
                //                gl_item_menu_config_sensores = eeprom_read(END_QTDE_SENSORES);
                //                qtd_sens = menu_quant_sensores[gl_item_menu_config_sensores].i_quant_sensores;
                posicao = eeprom_read(END_QTDE_SENSORES);
                qtd_sens = menu_quant_sensores[posicao].i_quant_sensores;

                //endereco = END_INICIO_AMOSTRAS + (gl_item_menu_ver_todos * 2);
                endereco = END_INICIO_AMOSTRAS + (gl_item_menu_ver_todos * qtd_sens * 2);
                //endereco = END_INICIO_AMOSTRAS;
                //MOSTRAR OS VALORES GRAVADOS NA EEPROM:
                for (uint8_t i = 0; i < qtd_sens; i++) {
                    //endereco = endereco + (i*2); //enderecos de 2 bytes
                    temp_int = eeprom_ler_word(endereco);

                    sprintf(tmp, "%d=%d.%d", i + 1, temp_int / 10, temp_int % 10);
                    lcd_goto(LCD_POSICAO[i].y, LCD_POSICAO[i].x);
                    lcd_puts(tmp);
                    endereco = endereco + 2; //enderecos de 2 bytes
                }
            }
            break; //EST_MOSTRA_TODOS
        case EST_MOSTRA_MAX_MIN:
            lcd_clear();
            maior = eeprom_ler_word(END_AMOSTRA_MAX);
            menor = eeprom_ler_word(END_AMOSTRA_MIN);

            if (maior == VALOR_MINIMO_AMOSTRA) {
                if (menor == VALOR_MAXIMO_AMOSTRA) {
                    lcd_puts("Nenhum Dado");
                }
            } else {
                sprintf(tmp, "Max=%d.%d", maior / 10, maior % 10);
                lcd_puts(tmp);
                lcd_goto(2, 0);
                sprintf(tmp, "Min=%d.%d", menor / 10, menor % 10);
                lcd_puts(tmp);
            }
            break; //EST_MOSTRA_MAX_MIN
        case EST_LIMPAR:
            eeprom_write(END_QTDE_VALORES, 0);
            eeprom_write(END_QTDE_SENSORES, QUANT_SENSORES_DEFAULT);
            eeprom_gravar_word(END_TX_AMOSTRA, TEMPO_AMOSTRA_DEFAULT);
            eeprom_gravar_word(END_AMOSTRA_MIN, VALOR_MAXIMO_AMOSTRA);
            eeprom_gravar_word(END_AMOSTRA_MAX, VALOR_MINIMO_AMOSTRA);
            for (uint8_t i = 0; i < QTD_MAX_AMOSTRAS * 2; i++) {
                eeprom_write(i + END_INICIO_AMOSTRAS, 0);
            }
            lcd_clear();
            lcd_puts("Tudo Limpo");
            break; //EST_LIMPAR
        case EST_MENU_CONF_TEMPO_AMOSTRA:
            //Atualiza tela:
            lcd_clear();
            lcd_puts(menu_tempo_amostra[gl_item_menu_config_amostra].str_tempo_amostra);
            break; //EST_MENU_CONF_TEMPO_AMOSTRA
#ifdef _DEBUG_
        case EST_ENVIAR_DADOS:
            bytes_tx = rs232_transmite();
            lcd_clear();
            sprintf(tmp, "%d bytes transmitidos", bytes_tx);
            lcd_puts(tmp);
            break; //EST_ENVIAR_DADOS
#endif
        case EST_MENU_CONF_QUANT_SENSORES:
            lcd_clear();
            lcd_puts(menu_quant_sensores[gl_item_menu_config_sensores].str_quant_sensores);
            break; //EST_MENU_CONF_QUANT_SENSORES
    } //switch (novo_estado)
    gl_estado = novo_estado;
}
