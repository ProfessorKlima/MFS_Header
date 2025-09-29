/* 
 * File:   schieberegister.h
 * Author: loki
 *
 * Created on September 18, 2025, 6:19 PM
 */

#ifndef SCHIEBEREGISTER_H
#define	SCHIEBEREGISTER_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define SR_DATA_DDR DDRB
#define SR_DATA_PORT PORTB
#define SR_DATA_PIN PB0
#define SR_CLK_DDR DDRD
#define SR_CLK_PORT PORTD
#define SR_CLK_PIN PD7
#define SR_OUT_CLK_DDR DDRD
#define SR_OUT_CLK_PORT PORTD
#define SR_OUT_CLK_PIN PD4

void init_sr_display(void);
void update_sr_display(int zahl);


#ifdef	__cplusplus
}
#endif

#endif	/* SCHIEBEREGISTER_H */

