/* 
 * File:   mq6.h
 * Author: ti
 *
 * Created on 6 de Janeiro de 2015, 22:27
 */

#ifndef MQ6_H
#define	MQ6_H

#ifdef	__cplusplus
extern "C" {
#endif

#define K 19.5897852088
#define INV_N -2.32192809474

    const uint16_t mq_gl_r0 = 378;

    uint16_t mq6_calibrar(void);
    void mq_mostra(uint16_t t_int, uint8_t i);

#ifdef	__cplusplus
}
#endif

#endif	/* MQ6_H */

