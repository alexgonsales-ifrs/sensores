/* 
 * File:   rs232.h
 * Author: ti
 *
 * Created on 22 de Janeiro de 2014, 05:10
 */

#ifndef RS232_H
#define	RS232_H

#ifdef	__cplusplus
extern "C" {
#endif


    /**
     * Inicializa a interface serial. E chamada durante a funcao main().
     */
    void rs232_init(void);

    /**
     * Transmite pela porta serial todos os valores de medicao gravados na eeprom.
     * @return quantidade de bytes transmitidos
     */
    uint8_t rs232_transmite(void);

#ifdef	__cplusplus
}
#endif

#endif	/* RS232_H */

