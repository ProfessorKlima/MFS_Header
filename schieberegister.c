#include "schieberegister.h"
#include "avr/io.h"
#include "avr/interrupt.h"

uint8_t seg1, seg2, seg3, seg4;

// Segmente, die leuchten sollen pro Zahlwert (Low-Aktiv), & 0x7F Verknüpfen fuer Dezimalpunkt
const uint8_t SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};

void short_delay(void) {
    for (volatile int i=0; i<10; i++)
        ;
}

void long_delay(void) {
    for (volatile int i=0; i<1000; i++)
        ;
}

void sr_clk(void) {
    SR_CLK_PORT &= ~(1<<SR_CLK_PIN);
    short_delay();
    SR_CLK_PORT |= (1<<SR_CLK_PIN);
    short_delay();
}

void sr_out_clk(void) {
    SR_OUT_CLK_PORT &= ~(1<<SR_OUT_CLK_PIN);
    short_delay();
    SR_OUT_CLK_PORT |= (1<<SR_OUT_CLK_PIN);
}

void init_sr_display(void) {
    seg1 = seg2 = seg3 = seg4 = 0;
    SR_DATA_DDR |= (1<<SR_DATA_PIN);
    SR_CLK_DDR |= (1<<SR_CLK_PIN);
    SR_OUT_CLK_DDR |= (1<<SR_OUT_CLK_PIN);
    
    SR_DATA_PORT &= ~(1<<SR_DATA_PIN);
    SR_CLK_PORT &= ~(1<<SR_CLK_PIN);
    SR_OUT_CLK_PORT &= ~(1<<SR_CLK_PIN);
    
    TCCR2A = (1 << WGM21);         // CTC Mode
    OCR2A = 32;                                         // Vergleichswert x = (CPU / (2 x Teiler x f)) - 1
    TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);   // CTC-Mode, Teiler = 1024
    TIMSK2 |= (1 << OCIE2A);                              // Output Compare A Match Interrupt Enable
    sei();                                                // IRQ Enable
}


void shift_out(uint8_t data) {
    for (volatile int8_t stelle = 7; stelle >= 0; stelle--) {
        if (data & (1 << stelle))
            SR_DATA_PORT |= (1<<SR_DATA_PIN);
        else
            SR_DATA_PORT &= ~(1<<SR_DATA_PIN);
        sr_clk();
        short_delay();
    }    
}


ISR(TIMER2_COMPA_vect) {
    static int8_t segnr = 0;

    // Output deaktivieren während geshifted wird
    SR_OUT_CLK_PORT &= ~(1<<SR_OUT_CLK_PIN);
    
    switch (segnr) {
        case 0:
            shift_out(SEGMENT_MAP[seg1]);
            shift_out(0x01);
            segnr++;
            break;
        case 1:
            shift_out(SEGMENT_MAP[seg2]);
            shift_out(0x02);
            segnr++;
            break;
        case 2:
            shift_out(SEGMENT_MAP[seg3]);
            shift_out(0x04);
            segnr++;
            break;
        case 3:
            shift_out(SEGMENT_MAP[seg4]);
            shift_out(0x08);
            segnr = 0;
            break;
    }
    
    // Output reaktivieren
    SR_OUT_CLK_PORT |= (1<<SR_OUT_CLK_PIN);
}

void update_sr_display(int zahl) {
    uint8_t tmp_seg1, tmp_seg2, tmp_seg3, tmp_seg4;
    
    // die Zwischenspeicher sind nötig, damit nicht Zwischenwerte während eines Timer-Interrupts verwendet werden
    
    tmp_seg1 = (zahl / 1000);
    tmp_seg2 = (zahl - 1000*tmp_seg1) / 100;
    tmp_seg3 = (zahl - 1000*tmp_seg1 - 100*tmp_seg2) / 10;
    tmp_seg4 = zahl % 10;
    
    seg1 = tmp_seg1;
    seg2 = tmp_seg2;
    seg3 = tmp_seg3;
    seg4 = tmp_seg4;
}
