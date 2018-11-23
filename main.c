#include <avr/io.h>
#include <avr/interrupt.h>
#include "SSD1306.h"
#include "main.h"



void init(void)
{
  /* -------- */
  /* I2C INIT */
  /* -------- */

  /* set prescaler to 1 */
  TWSR &= ~((1 << TWPS1) | (1 << TWPS0));

  /* set bitrate register */
  TWBR = ((F_CPU/F_SCL) - 16) / 2;



  /* ------------ */
  /* SSD1306 init */
  /* ------------ */

  const uint8_t ssd1306_init[] = {
    SSD1306_DISP_OFF,
    SSD1306_SET_MUX, 0x3F,
    SSD1306_SET_DISP_OFFSET, 0x00,
    SSD1306_SET_START_LINE | 0x00,
    SSD1306_SEG_REMAP | 0x01,
    SSD1306_COM_SCAN_INC,
    SSD1306_SET_COM_PINS, 0x12,
    SSD1306_SET_CONTRAST, 0xFF,
    SSD1306_ENTIRE_DISP_OFF,
    SSD1306_DISP_NORMAL,
    SSD1306_MEM_MODE, 0x01,
    SSD1306_SET_CLOCK_DIV, 0x80,
    SSD1306_CHARGE_PUMP, 0x14,
    SSD1306_DISP_ON
  };
  const uint8_t init_length = sizeof(ssd1306_init);

  /* perform initialization routine */
  uint8_t i;
  for (i = 0; i < init_length; i++)
    ssd1306_send_cmd(ssd1306_init[i]);



  /* -------- */
  /* ADC init */
  /* -------- */

  /* scale the clock to 16MHz / 128 = 125 kHz*/
  ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);

  /* set input voltage */
  ADMUX |= 1 << REFS0;

  /* input pin defaults to ADC0 */

  /* disable digital input on ADC0 */
  DIDR0 |= 1 << ADC0D;

  /* enable free run mode */
  ADCSRA |= 1 << ADATE;
  ADCSRB &= ~( (1 << ADTS0) | (1 << ADTS1) | (1 << ADTS0) );
}

int main(void)
{
  init();

  complex ADC_data[SAMPLES] = {};

#if SAMPLES <= 128
  uint8_t num_samples = 0;
#else
  uint16_t num_samples = 0;
#endif

  /* clears the frame */
  //ssd1306_update_frame(ADC_data);

  /* start the ADC */
  ADCSRA |= (1 << ADEN);
  ADCSRA |= (1 << ADSC);

  uint16_t delay;
  while (1)
  {
    while (num_samples < SAMPLES)
    {
      /* wait for ADC */
      while (!(ADCSRA&(1 << ADIF)));
      ADC_data[num_samples].Re = ADCL;
      ADC_data[num_samples].Re |= ADCH << 8;
      /* remove the DC bias */
      ADC_data[num_samples].Re -= 512;
      ADC_data[num_samples].Im = 0;
      num_samples += 1;

      /* this is here because my mic is too slow */
      /* it can and should be removed */
      for (delay = 0; delay < 800; delay++)
	asm volatile("nop");
    }
    num_samples = 0;

    /* do FFT */
    window(ADC_data);
    bit_reversal(ADC_data);
    fft(ADC_data);
    scale(ADC_data);

    /* update frame */
    ssd1306_update_frame(ADC_data);

    /* takes ~104 us for audio sample to be ready */
    /* so samples are ~9.6 kHz */
    /* meaning frequency range is approximately 0Hz - 4800Hz */

    /* discard old data and start new conversion */
    ADC_data[num_samples].Re = ADCL;
    ADC_data[num_samples].Re = ADCH;
  }

  return 0;
}
