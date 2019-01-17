#ifndef MAIN
#define MAIN

#include "SSD1306.h"

#define F_SCL 100000L
#define i2c_addr SSD1306_ADDRESS
#define SAMPLES 64

typedef struct {
  int16_t Re;
  int16_t Im;
} complex;

/* I2C.c */
void i2c_start(void);
void i2c_stop(void);
void i2c_send(uint8_t);

/* SSD1306.c */
void ssd1306_send_cmd(uint8_t);
void ssd1306_update_frame(int8_t*);

/* FFT.c */
void bit_reversal(complex*);
void window(complex*);
void fft(complex*);
void scale(complex*, int8_t*);
int16_t mul_16_bit(int16_t, int16_t);

/* main.c */
void init(void);
int main(void);

#endif
